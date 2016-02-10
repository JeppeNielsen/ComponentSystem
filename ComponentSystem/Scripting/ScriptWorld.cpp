//
//  ScriptWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptWorld.hpp"
#include <dlfcn.h>
#include "Logic.hpp"
#include "ScriptParser.hpp"
#include <set>
#include "IScriptSystem.hpp"

using namespace std;

vector<string> Compile(const string &cmd) {
    FILE*           fp;
    const int       SIZEBUF = 1234;
    char            buf [SIZEBUF];
    vector<string> out;
    
    if ((fp = popen(cmd.c_str (), "r")) == NULL) {
        return out;
    }
    string cur_string = "";
    while (fgets(buf, sizeof (buf), fp)) {
        cur_string += buf;
    }
    out.push_back(cur_string.substr (0, cur_string.size () - 1));
    pclose(fp);
    return out;
}

ScriptWorld::ScriptWorld() : libHandle(0) {
    ExtractWorldComponentNames();
}

void ScriptWorld::ExtractWorldComponentNames() {
    worldComponentNames.clear();
    GameWorldSettings::SerializableComponents serializableComponents;
    
    meta::for_each_in_tuple(serializableComponents, [this] (auto componentPointer) {
        using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
        worldComponentNames.push_back({ ComponentType{}.GetType().name, GameWorldSettings::GetComponentID<ComponentType>() });
    });
}



string ScriptWorld::ExtractHeaderPath(const std::string &headerFile) {
    size_t lastPath = headerFile.rfind("/");
    return headerFile.substr(0, lastPath);
}

string ScriptWorld::ExtractHeaderName(const std::string &headerFile) {
    size_t lastPath = headerFile.rfind("/");
    return headerFile.substr(lastPath + 1, headerFile.size() - lastPath - 1);
}

void ScriptWorld::SetFiles(const std::string& dynamicLibPath, const std::string& scriptingIncludeDir, const std::vector<std::string> &sourceFiles, const std::vector<std::string> &headerFiles) {
    
    this->sourceFiles.clear();
    this->headerNames.clear();
    this->headerPaths.clear();
    
    this->dynamicLibPath = dynamicLibPath;
    this->scriptingIncludeDir = scriptingIncludeDir;
    this->sourceFiles = sourceFiles;
    
    for(auto& header : headerFiles) {
        headerNames.push_back(ExtractHeaderName(header));
    }
    
    //header paths
    {
        std::set<std::string> uniqueHeaderPaths;
        
        uniqueHeaderPaths.insert("-I" + scriptingIncludeDir);
        for(auto& header : headerFiles) {
            uniqueHeaderPaths.insert("-I" + ExtractHeaderPath(header));
        }
        
        for(auto& source : sourceFiles) {
            uniqueHeaderPaths.insert("-I" + ExtractHeaderPath(source));
        }
        
        for(auto& uniqueHeader : uniqueHeaderPaths) {
            headerPaths.push_back(uniqueHeader);
        }
    }
    
    std::cout << "headerPaths : "<<std::endl;
    for(auto& s : headerPaths) {
        std::cout << s << std::endl;
    }
    
    std::cout << "headerNames : "<<std::endl;
    for(auto& s : headerNames) {
        std::cout << s << std::endl;
    }
}

bool ScriptWorld::Build() {

    std::string mainCppFile = scriptingIncludeDir + "/scriptWorld_autogenerated.cpp";
    ExtractScriptClasses();
    
    WriteMainCppFile(mainCppFile);
    
    string compilerPath ="/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++";
    string compilerFlags = "-dynamiclib -std=c++14 -stdlib=libc++";
    string outputFile = "-o " + dynamicLibPath;
    
    string compilerArgs = compilerPath + " " + compilerFlags + " ";
    
    for(auto& header : headerPaths) {
        compilerArgs += header + " ";
    }
    compilerArgs += mainCppFile + " ";
    compilerArgs += outputFile;
    
    auto out = Compile(compilerArgs);
    for(auto s : out) {
        std::cout << s << std::endl;
    }
    
    LoadLib();

    return false;
}

bool ScriptWorld::LoadLib() {
    
    libHandle = dlopen(dynamicLibPath.c_str(), RTLD_LAZY);
    
    if (!libHandle) {
        cerr << "Cannot open '" << dynamicLibPath << "' library: " << dlerror() << '\n';
        return false;
    }

    createSystem = (CreateSystem)dlsym(libHandle, "CreateSystem");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CreateSystem': " << dlsym_error << '\n';
        UnloadLib();
        return false;
    }
    
    countSystems = (CountSystems) dlsym(libHandle, "CountSystems");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CountSystems': " << dlsym_error << '\n';
        UnloadLib();
        return false;
    }
    
    deleteSystem = (DeleteSystem) dlsym(libHandle, "DeleteSystem");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'DeleteSystem': " << dlsym_error << '\n';
        UnloadLib();
        return false;
    }
    
    createComponent = (CreateComponent) dlsym(libHandle, "CreateComponent");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CreateComponent': " << dlsym_error << '\n';
        dlclose(libHandle);
        return false;
    }
    
    countComponents = (CountComponents) dlsym(libHandle, "CountComponents");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CountComponents': " << dlsym_error << '\n';
        dlclose(libHandle);
        return false;
    }
    
    deleteComponent = (DeleteComponent) dlsym(libHandle, "DeleteComponent");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'DeleteComponent': " << dlsym_error << '\n';
        dlclose(libHandle);
        return false;
    }
    
    getTypeInfo = (GetTypeInfo) dlsym(libHandle, "GetTypeInfo");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'GetTypeInfo': " << dlsym_error << '\n';
        dlclose(libHandle);
        return false;
    }
    
    return true;
}

void ScriptWorld::UnloadLib() {
    if (!libHandle) return;
    dlclose(libHandle);
}
void ScriptWorld::ExtractScriptClasses() {
    ScriptClass allClasses;

    ScriptParser parser;

    parser.ParseCode(
        allClasses,
        sourceFiles[0],
        headerPaths
    );

    //allClasses.Print();
    scriptClasses = parser.ExtractSystemsAndComponents(allClasses);
}

void ScriptWorld::WriteMainCppFile(const std::string &path) {
    ofstream file;
    file.open(path);
    
    WriteMainGameObject(file);
    WriteMainIncludes(file);
    WriteMainSystems(file);
    WriteMainComponents(file);
    WriteMainSerializedComponents(file);
    
    file.close();
}

void ScriptWorld::WriteMainIncludes(std::ofstream &file) {
    
    auto& systems = scriptClasses.children["Systems"].children;
    
    std::set<std::string> uniqueIncludes;
    for(auto& system : systems) {
        uniqueIncludes.insert(system.second.sourceFile);
    }
    
    file<<"#include \"TypeInfo.hpp\""<<std::endl;
    
    for(auto& headerName : headerNames) {
        file<<"#include \""<<headerName<<"\""<<std::endl;
    }
    
    for(auto& include : uniqueIncludes) {
        file << "#include \"" << include << "\""<< std::endl;
    }
    file<<std::endl;
    
}

void ScriptWorld::WriteMainGameObject(std::ofstream &file) {

    auto& scriptComponents = scriptClasses.children.find("Components")->second.children;
    
    for(auto& componentName : worldComponentNames) {
        file << "struct "<< componentName.name << ";"<<std::endl;
    }
        
    for (auto component : scriptComponents) {
        file << "struct "<< component.second.name << ";"<<std::endl;
    }
    
    file << std::endl;
    
    file << "class GameObject {"<<std::endl;
    file << "private:"<<std::endl;
    file << "    virtual void* GetComponent(int componentID) = 0;"<<std::endl;
    file << "    virtual void* AddComponent(int componentID) = 0;"<<std::endl;
    file << "    virtual void RemoveComponent(int componentID) = 0;"<<std::endl;
    file << "    virtual void* GetScriptComponent(int componentID) = 0; "<< std::endl;
    file << "    virtual void* AddScriptComponent(int componentID) = 0; "<< std::endl;
    file << "    virtual void RemoveScriptComponent(int componentID) = 0; "<< std::endl;
    file << "public:" << std::endl;
    file << "    template<typename T> T* GetComponent() { return (T*)0; }"<<std::endl;
    file << "    template<typename T> T* AddComponent() { return (T*)0; }"<<std::endl;
    file << "    template<typename T> void RemoveComponent() { }"<<std::endl;
    file << "};"<<std::endl;
    
    for(auto& componentName : worldComponentNames) {
        file<<"template<> " << componentName.name  << "* GameObject::GetComponent<"<< componentName.name << ">() { return ("<< componentName.name <<"*) GetComponent("<<componentName.index<<"); }"<<std::endl;
    }
    
    for(auto& componentName : worldComponentNames) {
        file<<"template<> " << componentName.name  << "* GameObject::AddComponent<"<< componentName.name << ">() { return ("<< componentName.name <<"*) AddComponent("<<componentName.index<<"); }"<<std::endl;
    }
    
    for(auto& componentName : worldComponentNames) {
        file<<"template<> void GameObject::RemoveComponent<"<< componentName.name << ">() { RemoveComponent("<<componentName.index<<"); }"<<std::endl;
    }
    
    int index = 0;
    for (auto it : scriptComponents) {
        auto& component = it.second;
        file<<"template<> " << component.name  << "* GameObject::GetComponent<"<< component.name << ">() { return ("<< component.name <<"*) GetScriptComponent("<<index<<"); }"<<std::endl;
        
        file<<"template<> " << component.name  << "* GameObject::AddComponent<"<< component.name << ">() { return ("<< component.name <<"*) AddScriptComponent("<<index<<"); }"<<std::endl;
        
        file<<"template<> void GameObject::RemoveComponent<"<< component.name << ">() { RemoveScriptComponent("<<index<<"); }"<<std::endl;
        index++;
    }
}

void ScriptWorld::WriteMainSystems(std::ofstream &file) {
    
    auto& systems = scriptClasses.children["Systems"].children;
    
    {
       file<<"extern \"C\" int CountSystems() {"<<std::endl;
       file<<"   return "<< systems.size() <<";"<<std::endl;
       file<<"}"<<std::endl;
    }
    
    {
        file<<"extern \"C\" IScriptSystem* CreateSystem(int systemID) {"<<std::endl;
            file << "   switch (systemID) { " << std::endl;
                int index = 0;
                for(auto& system : systems) {
                    file<<"      case "<<index <<":"<<" return new "<<system.second.name<<"();"<<std::endl;
                    index++;
                }
        file<<"      default: return 0;"<<std::endl;
        file<<"   }"<<std::endl;
        file<<"}"<<std::endl;
    }
    
    {
       file<<"extern \"C\" void DeleteSystem(IScriptSystem* scriptSystem) {"<<std::endl;
       file<<"   delete scriptSystem; "<<std::endl;
       file<<"}"<<std::endl;
    }
}

void ScriptWorld::WriteMainComponents(std::ofstream &file) {
    
    auto& components = scriptClasses.children["Components"].children;

    {
       file<<"extern \"C\" int CountComponents() {"<<std::endl;
       file<<"   return "<< components.size() <<";"<<std::endl;
       file<<"}"<<std::endl;
    }
    
    {
        file<<"extern \"C\" void* CreateComponent(int componentID) {"<<std::endl;
            file << "   switch (componentID) { " << std::endl;
                int index = 0;
                for(auto& component : components) {
                    file<<"      case "<<index <<":"<<" return new "<<component.second.name<<"();"<<std::endl;
                    index++;
                }
        file<<"      default: return 0;"<<std::endl;
        file<<"   }"<<std::endl;
        file<<"}"<<std::endl;
    }
    
    {
       file<<"extern \"C\" void DeleteComponent(int componentID, void* component) {"<<std::endl;
            file << "   switch (componentID) { " << std::endl;
                int index = 0;
                for(auto& component : components) {
                    file<<"      case "<<index <<":"<<" { delete (("<<component.second.name<<"*)component); break; }"<<std::endl;
                    index++;
                }
        file<<"   }"<<std::endl;
        file<<"}"<<std::endl;
    }

    file<<std::endl;
}

void ScriptWorld::WriteMainSerializedComponents(std::ofstream &file) {

    auto& components = scriptClasses.children["Components"].children;

    file<<"extern \"C\" TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {"<<std::endl;
        file << "   switch (componentID) { " << std::endl;
            int index = 0;
            for(auto& componentIt : components) {
                auto& component = componentIt.second;
                file<<"      case "<<index <<": {"<<std::endl;
                file<<"      "<<component.name<<"* component = ("<<component.name<<"*)componentPtr;"<<std::endl;
                file<<"      TypeInfo* info = new TypeInfo();"<<std::endl;
                
                for(auto& field : component.fields) {
                    if (field.type == "int" ||
                        field.type == "string" ||
                        field.type == "float") {
                file<<"      info->AddField(component->"<< field.name <<", \""<<field.name<<"\");"<<std::endl;
                    }
                }
                file<<"      return info;"<<std::endl;
                
                file<<"      break; }"<<std::endl;
                 // return new "<<system.second.name<<"();"<<std::endl;
                index++;
            }
    file<<"      default: return 0;"<<std::endl;
    file<<"   }"<<std::endl;
    file<<"}"<<std::endl;

}

bool ScriptWorld::FindComponentIndex(std::string componentName, bool &staticComponent, int& index) {
    auto& scriptComponents = scriptClasses.children["Components"].children;
    {
        index = 0;
        for(auto& scriptComponent : scriptComponents) {
            if (scriptComponent.second.name == componentName) {
                staticComponent = false;
                return true;
            }
            index++;
        }
    }
    
    index = -1;
    
    for(auto& worldComponentName : worldComponentNames) {
        if (componentName == worldComponentName.name) {
            index = worldComponentName.index;
            break;
        }
    }
    
    if (index>=0) {
        staticComponent = true;
        return true;
    }
    
    return false;
}

template<>
Container<void*>::ObjectInstance* Container<void*>::CreateInstance() {
    typedef void* (*CreateComponent)(int);
    CreateComponent* createComponent = (CreateComponent*)createContext;
    CreateComponent func = (*(createComponent));
    ObjectInstance* instance = new ObjectInstance();
    instance->object = func(contextIndex);
    return instance;
}

template<>
void Container<void*>::DeleteInstance(Container<void *>::ObjectInstance *instance) {
    typedef void* (*DeleteComponent)(int, void*);
    DeleteComponent* deleteComponent = (DeleteComponent*)deleteContext;
    DeleteComponent func = (*(deleteComponent));
    instance->object = func(contextIndex, instance->object);
}

void ScriptWorld::RemoveGameWorld(GameWorld &world) {

}


void ScriptWorld::AddGameWorld(GameWorld &world) {
    int numberOfSystems = countSystems();
    int numberOfComponents = countComponents();
    
    std::vector<IScriptSystem*> scriptSystems;
    
    for (int i=0; i<numberOfSystems; ++i) {
        scriptSystems.push_back(createSystem(i));
    }
    
    world.dynamicScriptSystemComponents.resize(numberOfComponents);
    world.scriptSystems = scriptSystems;
    {
        auto& scriptSystems = scriptClasses.children["Systems"].children;
        
        int index = 0;
        for (auto& scriptSystem : scriptSystems) {
            GameWorld::ScriptSystemData data;
            for (auto& component : scriptSystem.second.templateArguments) {
                int componentIndex;
                bool staticComponent;
                if (FindComponentIndex(component, staticComponent, componentIndex)) {
                    if (staticComponent) {
                        world.staticScriptSystemComponents[componentIndex].push_back(index);
                        data.staticComponents[componentIndex] = true;
                    } else {
                        world.dynamicScriptSystemComponents[componentIndex].push_back(index);
                        data.scriptComponents.push_back(componentIndex);
                    }
                }
            }
            world.scriptSystemsData.push_back(data);
            index++;
        }
    }
    
    
    world.scriptComponents.resize(numberOfComponents);
    for (int i=0; i<numberOfComponents; i++) {
        auto& scriptContainer = world.scriptComponents[i];
        scriptContainer.createContext = &createComponent;
        scriptContainer.deleteContext = &deleteComponent;
        scriptContainer.contextIndex = i;
    }
    
    

}




