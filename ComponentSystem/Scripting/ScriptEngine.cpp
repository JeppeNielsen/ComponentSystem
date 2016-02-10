//
//  ScriptEngine.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 25/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptEngine.hpp"
#include <iostream>
#include <dlfcn.h>
#include "Logic.hpp"
#include <fstream>
#include "IScriptSystem.hpp"
#include "ScriptParser.hpp"
#include <sstream>

using namespace std;

vector<string> ScriptEngine::Compile(const string &cmd) {
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

bool ScriptEngine::BuildDir(const std::string &dir) {
    
    {
        ScriptClass scriptClasses;
        scriptClasses.name = "Root";

        ScriptParser parser;
    
        parser.ParseCode
        (   scriptClasses,
            "/Projects/ComponentSystem/ComponentSystem/LibCode/ScriptTest.hpp",
            {
                "-I/Projects/ComponentSystem/ComponentSystem/LibCode",
                "-I/Projects/ComponentSystem/ComponentSystem/Scripting"
            }
        );

        scriptClasses.Print();
        
        scriptWorld = parser.ExtractSystemsAndComponents(scriptClasses);
        parser.CreateScriptHeader(scriptWorld, "/Projects/ComponentSystem/ComponentSystem/LibCode/ScriptEngine_autoGenerated.cpp");
    }

    WriteComponentIDsHeader("/Projects/ComponentSystem/ComponentSystem/LibCode/libGameObject.h");

    auto out = Compile(
        "/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++ "
        "-dynamiclib "
        "-std=c++14 -stdlib=libc++ "
        //" -I/Projects/ComponentSystem/ComponentSystem/ComponentSystem/ "
        //" -I/Projects/ComponentSystem/ComponentSystem/meta/ "
        //" -I/Projects/ComponentSystem/ComponentSystem/Data/ "
        //" -I/Projects/ComponentSystem/ComponentSystem/Serialization/ "
        //" -I/Projects/ComponentSystem/ComponentSystem/Scripting/ "
        //" -I/Projects/ComponentSystem/ComponentSystem/tests/ "
        " -I/Projects/ComponentSystem/ComponentSystem/LibCode/ "
        " -I/Projects/ComponentSystem/ComponentSystem/Scripting/ "
        " /Projects/ComponentSystem/ComponentSystem/LibCode/ScriptEngine_autoGenerated.cpp "
        //" /Projects/ComponentSystem/ComponentSystem/tests/Logic.cpp "
        " engine.so "
        " -o libCode.so"// -O3"
        );
    for(auto s : out) {
        std::cout << s << std::endl;
    }
    return out.size() == 0;
}

void ScriptEngine::WriteComponentIDsHeader(const std::string &path) {
    
    GameWorld world;
    
    auto& scriptComponents = scriptWorld.children.find("Components")->second.children;
    
    std::ofstream file;
    file.open(path);
    
    file << "#pragma once"<< std::endl;
    //file << "template<typename T> int GetComponentID() { return 0; }"<<std::endl;
    
    world.EnumerateComponentClasses([&file] (std::string name, int index) {
        file << "struct "<< name << ";"<<std::endl;
    });
    
    for (auto component : scriptComponents) {
        file << "struct "<< component.second.name << ";"<<std::endl;
    }
    
    /*
    world.EnumerateComponentClasses([&file] (std::string name, int index) {
        file << "template<> int GetComponentID<"<< name << ">() { return "<<index<<"; }"<<std::endl;
    });
    */
    
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
    
    world.EnumerateComponentClasses([&file] (std::string name, int index) {
        file<<"template<> " << name  << "* GameObject::GetComponent<"<< name << ">() { return ("<< name <<"*) GetComponent("<<index<<"); }"<<std::endl;
    });
    
    world.EnumerateComponentClasses([&file] (std::string name, int index) {
        file<<"template<> " << name  << "* GameObject::AddComponent<"<< name << ">() { return ("<< name <<"*) AddComponent("<<index<<"); }"<<std::endl;
    });
    
    world.EnumerateComponentClasses([&file] (std::string name, int index) {
        file<<"template<> void GameObject::RemoveComponent<"<< name << ">() { RemoveComponent("<<index<<"); }"<<std::endl;
    });
    
    int index = 0;
    for (auto it : scriptComponents) {
        auto& component = it.second;
        file<<"template<> " << component.name  << "* GameObject::GetComponent<"<< component.name << ">() { return ("<< component.name <<"*) GetScriptComponent("<<index<<"); }"<<std::endl;
        
        file<<"template<> " << component.name  << "* GameObject::AddComponent<"<< component.name << ">() { return ("<< component.name <<"*) AddScriptComponent("<<index<<"); }"<<std::endl;
        
        file<<"template<> void GameObject::RemoveComponent<"<< component.name << ">() { RemoveScriptComponent("<<index<<"); }"<<std::endl;
        index++;
    }
    
    file.close();
}

/*
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
*/

bool ScriptEngine::Run() {
    
    void* handle = dlopen("./libCode.so", RTLD_LAZY);
    
    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return false;
    }
    
    cout << "Loading symbol hello.so...\n";
    
    
    
    
    typedef IScriptSystem* (*CreateSystem)(int);
    typedef int (*CountSystems)();
    typedef void (*DeleteSystem)(IScriptSystem*);
    
    dlerror();
    CreateSystem createSystem = (CreateSystem) dlsym(handle, "CreateSystem");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CreateSystem': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    CountSystems countSystems = (CountSystems) dlsym(handle, "CountSystems");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CountSystems': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    DeleteSystem deleteSystem = (DeleteSystem) dlsym(handle, "DeleteSystem");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'DeleteSystem': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    typedef void* (*CreateComponent)(int);
    typedef int (*CountComponents)();
    typedef void (*DeleteComponent)(int, void*);
    
    CreateComponent createComponent = (CreateComponent) dlsym(handle, "CreateComponent");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CreateComponent': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    CountComponents countComponents = (CountComponents) dlsym(handle, "CountComponents");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'CountComponents': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    DeleteComponent deleteComponent = (DeleteComponent) dlsym(handle, "DeleteComponent");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'DeleteComponent': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    typedef TypeInfo* (*GetTypeInfo)(int, void*);
    GetTypeInfo getTypeInfo = (GetTypeInfo) dlsym(handle, "GetTypeInfo");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'GetTypeInfo': " << dlsym_error << '\n';
        dlclose(handle);
        return false;
    }
    
    
    
    int numberOfSystems = countSystems();
    int numberOfComponents = countComponents();
    
    std::vector<IScriptSystem*> scriptSystems;
    
    for (int i=0; i<numberOfSystems; ++i) {
        scriptSystems.push_back(createSystem(i));
    }
    
    {
        GameWorld world;
        
        world.dynamicScriptSystemComponents.resize(numberOfComponents);
        world.scriptSystems = scriptSystems;
        {
            auto& scriptSystems = scriptWorld.children["Systems"].children;
            
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
        
        GameObject* object = world.CreateObject();
        
        void* ptr = object->AddScriptComponent(0);
        object->AddComponent<Transform>()->x = 20;
        object->AddComponent<Velocity>()->x = 100;
        
        world.Update(0.01f);
        
        TypeInfo typeInfo;
        typeInfo.UpdateFromPointer(getTypeInfo(0, ptr));
        
        {
            
            
            minijson::writer_configuration config;
            config = config.pretty_printing(true);
            minijson::object_writer writer(std::cout, config);
            
            typeInfo.Serialize(writer);
            
            //field->Serialize(writer);
            writer.close();
            
            std::stringstream jsonStream;
            
            jsonStream<<"{ \"name\": \"Modified From Editor!\","
                        "\"clickedImageNo\": 6,"
                        "\"height\": 0.05 }";
            
            minijson::istream_context context(jsonStream);
            
            typeInfo.Deserialize(context);
        }
        
        world.Update(0.01);
        
        std::cout << " x: "<< object->GetComponent<Transform>()->x << std::endl;
        
        
        /*
        object->RemoveScriptComponent(0);
        object->RemoveComponent<Transform>();
        
        
        world.Update(0.01);
        */
        
        /*
        for (int i=0; i<numberOfSystems; i++) {
            
            IScriptSystem* system = createSystem(i);
            
            system->ObjectAdded(object);
            
            world.Update(1.0f);
            system->Update(1.0f);
            
            auto transform = object->GetComponent<Transform>();
            
            std::cout << " Transform::x = "<< transform->x<< std::endl;
        }
        */
        
        
    }
    
    for(auto scriptSystem : scriptSystems) {
        deleteSystem(scriptSystem);
    }
    
    // close the library
    cout << "Closing library...\n";
    dlclose(handle);

    return true;
}

bool ScriptEngine::FindComponentIndex(std::string componentName, bool &staticComponent, int& index) {
    auto& scriptComponents = scriptWorld.children["Components"].children;
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
    
    GameWorld world;
    
    index = -1;
    world.EnumerateComponentClasses([&index, componentName] (std::string name, int componentIndex) {
        if (name == componentName) {
            index = componentIndex;
        }
    });
    
    if (index>=0) {
        staticComponent = true;
        return true;
    }
    
    return false;
}
