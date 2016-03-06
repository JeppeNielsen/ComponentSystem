//
//  ScriptWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "ScriptClass.hpp"
#include "GameWorld.hpp"

class IScriptSystem;
class TypeInfo;

class ScriptWorld {
public:

    ScriptWorld();
    
    void SetFiles(
                  const std::string& dynamicLibPath,
                  const std::string& scriptingIncludeDir,
                  const std::vector<std::string>& sourceFiles,
                  const std::vector<std::string>& headerFiles);
    
    template<typename T>
    void SetWorldType() {
        auto componentNames = T::GetComponentNames();
        worldComponentNames.clear();
        for(int i=0; i<componentNames.size(); ++i) {
            worldComponentNames.push_back({ componentNames[i], i });
        }
    }
    
    bool Build();
    bool LoadLib();
    void UnloadLib();
    
    void AddGameWorld(GameWorld& world);
    void RemoveGameWorld(GameWorld& world);
    TypeInfo GetTypeInfo(GameObject& object, int index);
private:
    void ExtractScriptClasses();
    void WriteMainIncludes(std::ofstream &file);
    void WriteMainCppFile(const std::string& path);
    void WriteMainGameObject(std::ofstream& file);
    void WriteMainSystems(std::ofstream& file);
    void WriteMainComponents(std::ofstream& file);
    void WriteMainSerializedComponents(std::ofstream& file);
    
    std::string ExtractHeaderPath(const std::string& headerFile);
    std::string ExtractHeaderName(const std::string& headerFile);
    
    bool FindComponentIndex(std::string componentName, bool& staticComponent, int& index);
    
    std::string dynamicLibPath;
    std::string scriptingIncludeDir;
    std::vector<std::string> sourceFiles;
    std::vector<std::string> headerNames;
    std::vector<std::string> headerPaths;
    
    struct ComponentName {
        std::string name;
        int index;
    };
    
    std::vector<ComponentName> worldComponentNames;
    ScriptClass scriptClasses;
    
    using LibHandle = void*;
    
    LibHandle libHandle;
    
    typedef IScriptSystem* (*CreateSystem)(int);
    typedef int (*CountSystems)();
    typedef void (*DeleteSystem)(IScriptSystem*);
    
    typedef void* (*CreateComponent)(int);
    typedef int (*CountComponents)();
    typedef void (*DeleteComponent)(int, void*);
    typedef void (*ResetComponent)(int, void*, void*);
    
    typedef TypeInfo* (*GetTypeInfoFunction)(int, void*);
    typedef void (*DeleteTypeInfo)(TypeInfo*);
    
    CreateSystem createSystem;
    CountSystems countSystems;
    DeleteSystem deleteSystem;
    
    CreateComponent createComponent;
    CountComponents countComponents;
    DeleteComponent deleteComponent;
    ResetComponent resetComponent;
    
    GetTypeInfoFunction getTypeInfo;
    DeleteTypeInfo deleteTypeInfo;
};