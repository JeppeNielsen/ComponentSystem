//
//  GameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include <type_traits>
#include <assert.h>
#include "GameObject.hpp"
#include "IDHelper.hpp"
#include "MetaLibrary.hpp"
#ifdef SCRIPTING_ENABLED
#include "IScriptSystem.hpp"
#endif
#include "GameSystem.hpp"
#include "Container.hpp"


template<typename... Systems>
class GameWorldInitializer {
public:
    std::vector<IGameSystem*> CreateSystems() const {
        std::vector<IGameSystem*> systems;
        std::tuple<Systems*...> systemsTuple;
        Meta::for_each_in_tuple_non_const(systemsTuple, [&systems](auto t) {
            using SystemType = std::remove_pointer_t<decltype(t)>;
            int systemID = IDHelper::GetSystemID<SystemType>();
            if (systemID>=systems.size()) {
                systems.resize(systemID + 1, 0);
            }
            if (!systems[systemID]) {
                systems[systemID] = new SystemType();
            }
        });
        return systems;
    }
};

class GameWorld {
private:

    using Bitset = std::bitset<64>;

    friend class GameObject;
    using Objects = Container<GameObject>;
    
    using Systems = std::vector<IGameSystem*>;
    using SystemBitsets = std::vector<Bitset>;
    
    using Components = std::vector<void*>;
    using ComponentNames = std::vector<std::string>;
    using ComponentSystems = std::vector<std::vector<IGameSystem*>>;
    
    using Actions = std::vector<std::function<void()>>;
    
    Objects objects;
    
    Systems systems;
    SystemBitsets systemBitsets;
    
    Components components;
    ComponentNames componentNames;
    ComponentSystems componentSystems;
    
    Actions createActions;
    Actions removeActions;
    
    
    
    void InitializeSystemBitsets() {
        /*meta::for_each_in_tuple(systems, [&](auto& system) {
            using SystemType = std::remove_const_t<std::remove_reference_t<decltype(system)>>;
            meta::for_each_in_tuple(meta::IteratorPointer<typename SystemType::Components>{}.Iterate(), [&,this](auto component) {
                systemBitsets[Settings::template GetSystemID<SystemType>()]
                             [Settings::template GetComponentID<std::remove_pointer_t< decltype(component) >>()] = true;
            });
        });
        */
    }
    
    void InitializeComponentNames() {
        //componentNames = Settings::GetComponentNames();
    }
    
    void InitializeCommands() {
    /*
        meta::for_each_in_tuple(components, [this] (auto& component) {
            using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
            int componentIndex = IDHelper::GetComponentID<ComponentType>();
            if (componentIndex>=commands.size()) {
                commands.resize(componentIndex+1);
            }
            commands[componentIndex].addComponent = [](GameObject* gameObjectPtr) {
              GameObject* go = (GameObject*)gameObjectPtr;
              return go->template AddComponent<ComponentType>();
            };
            commands[componentIndex].addComponentReference = [](GameObject* gameObjectPtr, GameObject* source) {
              GameObject* go = (GameObject*)gameObjectPtr;
              return go->template AddComponent<ComponentType>((GameObject*) source);
            };
            commands[componentIndex].removeComponent = [](void* gameObjectPtr) -> void* {
              GameObject* go = (GameObject*)gameObjectPtr;
              go->template RemoveComponent<ComponentType>();
              return 0;
            };
        });
        
        meta::for_each_in_tuple(systems, [&](auto& system) {
            using SystemType = std::remove_const_t<std::remove_reference_t<decltype(system)>>;
            
            int systemIndex = IDHelper::GetSystemID<SystemType>();
            if (systemIndex>=getSystemCommands.size()) {
                getSystemCommands.resize(systemIndex+1);
            }
            getSystemCommands[systemIndex] = [this]() -> void* {
                return &GetSystem<SystemType>();
            };
        });
        */
    }
    
    /*
    GameObject* LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated) {
        GameObject* object = 0;
         minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string name = n;
            if (name == "GameObject" && v.type() == minijson::Object) {
                object = (GameObject*)CreateObject();
                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                    std::string name = n;
                    if (name == "Components" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            if (v.type() == minijson::Object) {
                                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                    object->AddComponent(context, n);
                                });
                            }
                        });
                    } else if (name == "Children" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            GameObject* child = LoadObject(context, onCreated);
                            if (child) {
                                child->Parent = object;
                            }
                        });
                    }
                    
                    if (onCreated) {
                        onCreated(object);
                    }
                });
            }
        });
        return object;
    }
    */
    
public:

    template<typename Initializer>
    void Initialize(const Initializer& initializer) {
        systems = initializer.CreateSystems();
        for(auto s : systems) {
            s->Initialize(this);
        }
    }

    template<typename System>
    System& GetSystem() {
        return *((System*)systems[IDHelper::GetSystemID<System>()]);
    }

    GameObject* CreateObject() {
        auto object = objects.CreateObjectNoReset();
        object->object.instance = object;
        object->object.SetWorld(this);
        object->object.Reset();
        return &object->object;
    }
    
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) {
        minijson::istream_context context(jsonStream);
        GameObject* object = 0;
        /*try {
            object = LoadObject(context, onCreated);
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
        */
        return object;
    }
    
    GameWorld() {
        InitializeSystemBitsets();
        InitializeComponentNames();
        InitializeCommands();
        objects.Initialize();
        //meta::for_each_in_tuple_non_const(components, [](auto& container) {
        //    container.Initialize();
        //});
    }

    void Initialize() {
        /*meta::for_each_in_tuple(initializeSystems, [&](auto system) {
            std::get<
                Settings::template GetSystemID<
                    std::remove_pointer_t<decltype(system)>
                >()
            >(systems).Initialize(this);
        });
        */
    }

    void Update(float dt) {
        DoActions(createActions);
        DoActions(removeActions);
        
        for(auto system : systems) {
            system->Update(dt);
        }
        
        /*meta::for_each_in_tuple(updateSystems, [this, dt] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Update(dt);
        });
        */
        
#ifdef SCRIPTING_ENABLED
        for(auto scriptSystem : scriptSystems) {
            scriptSystem->Update(dt);
        }
#endif
    }
    
    void Render() {
        for(auto system : systems) {
            system->Render();
        }
        /*
        meta::for_each_in_tuple(renderSystems, [this] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Render();
        });
        */
    }

    void DoActions(Actions& list) {
       for(int i=0; i<list.size(); ++i) {
            list[i]();
       }
       list.clear();
    }

    int ObjectCount() {
        return objects.Size();
    }
    
    GameObject* GetObject(int index) {
        return objects.Get(index);
    }

    void Clear() {
        for(int i=0; i<objects.Size(); ++i) {
            objects.Get(i)->Remove();
        }
        DoActions(createActions);
        DoActions(removeActions);
    }
    
    ~GameWorld() {
        Clear();
    }
    
//    void EnumerateComponentClasses(std::function<void(std::string, int)> callback) {
//        meta::for_each_in_tuple(serializableComponents, [this, callback] (auto componentPointer) {
//            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
//            callback(componentNames[Settings::template GetComponentID<ComponentType>()], Settings::template GetComponentID<ComponentType>());
//        });
//    }
    
#ifdef SCRIPTING_ENABLED
    //Scripting
    using StaticScriptSystemComponents = std::array<std::vector<short>, typename Settings::UniqueComponents{}.size()>;
    StaticScriptSystemComponents staticScriptSystemComponents;
    using ScriptSystemComponents = std::vector<std::vector<short>>;
    ScriptSystemComponents dynamicScriptSystemComponents;
    
    using ScriptComponents = std::vector<Container<void*>>;
    ScriptComponents scriptComponents;
    
    using ScriptBitset = std::vector<bool>;
    
    struct ScriptSystemData {
        typename Settings::Bitset staticComponents;
        std::vector<short> scriptComponents;
    };
    
    using ScriptSystems = std::vector<IScriptSystem*>;
    ScriptSystems scriptSystems;
    
    using ScriptSystemsData = std::vector<ScriptSystemData>;
    ScriptSystemsData scriptSystemsData;
    
    void ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted) {
        for(int i=0; i<ObjectCount(); ++i) {
            GetObject(i)->ClearScriptingData();
        }

        for(int i=0; i<typename Settings::UniqueComponents{}.size(); ++i) {
            staticScriptSystemComponents[i].clear();
        }
        dynamicScriptSystemComponents.clear();
        scriptComponents.clear();
        for(auto scriptSystem : scriptSystems) {
            onSystemDeleted(scriptSystem);
        }
        scriptSystems.clear();
        scriptSystemsData.clear();
    }

    void InitializeScriptData(int numSystems, int numComponents,
                std::function<IScriptSystem*(int)> onSystemCreate,
                std::function<void(Container<void*>&, int)> onComponentCreate,
                std::function<void(
                            StaticScriptSystemComponents& staticScriptSystemComponents,
                            ScriptSystemComponents& dynamicScriptSystemComponents,
                            ScriptSystemsData& scriptSystemsData)> onSystemData
                
                ) {
        
        for(int i=0; i<numSystems; i++) {
            scriptSystems.push_back(onSystemCreate(i));
        }
        
        scriptComponents.resize(numComponents);
        for(int i=0; i<numComponents; ++i) {
            onComponentCreate(scriptComponents[i], i);
        }
        
        dynamicScriptSystemComponents.resize(numComponents);
        onSystemData(staticScriptSystemComponents, dynamicScriptSystemComponents, scriptSystemsData);
        
        for(int i=0; i<ObjectCount(); ++i) {
            GetObject(i)->InitializeScriptingData();
        }
    }
#endif
};



template<typename Component>
void GameObject::RemoveComponent() {
    if (removedComponents[IDHelper::GetComponentID<Component>()]) return;
    assert(HasComponent<Component>());
    removedComponents[IDHelper::GetComponentID<Component>()] = true;
    
    world->removeActions.emplace_back([this]() {
        auto activeComponentsBefore = activeComponents;
        
        auto& systemsUsingComponent = world->componentSystems[IDHelper::GetComponentID<Component>()];
        for(auto system : systemsUsingComponent) {
            auto& bitSet = world->systemBitsets[system->index];
            bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
            if (wasInterest) {
            
                system->ObjectRemoved(this);
                
                int objectIndexInSystem = systemIndices[system->index];
                int lastIndex = (int)system->objects.size() - 1;
                GameObject* lastObject = (GameObject*)system->objects[lastIndex];
                lastObject->systemIndices[system->index]=objectIndexInSystem;
                system->objects[objectIndexInSystem]=lastObject;
                system->objects.pop_back();
            }
        }
        
#ifdef SCRIPTING_ENABLED
        auto activeScriptComponentsBefore = activeScriptComponents;
        CheckForScriptSystemsRemoval(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
        
        int componentID = IDHelper::GetComponentID<Component>();
        
        activeComponents[componentID] = false;
        removedComponents[componentID] = false;
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[componentID];
        Container<Component>* container = (Container<Component>*)world->components[componentID];
        container->RemoveObject(instance);
        components[componentID] = 0;
    });
}

template<typename Component>
Component* GameObject::AddComponent() {
    assert(!HasComponent<Component>());
    Container<Component>* container = (Container<Component>*)world->components[IDHelper::GetComponentID<Component>()];
    SetComponent<Component>(container->CreateObject());
    return GetComponent<Component>();
}

template<typename Component>
Component* GameObject::AddComponent(GameObject* source) {
    assert(source);
    assert(!HasComponent<Component>());
    assert(source->HasComponent<Component>());
    typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)source->components[IDHelper::GetComponentID<Component>()];
    ++instance->references;
    SetComponent<Component>(instance);
    return GetComponent<Component>();
}

template<typename Component>
void GameObject::SetComponent(typename Container<Component>::ObjectInstance *instance) {
    activeComponents[IDHelper::GetComponentID<Component>()] = true;
    components[IDHelper::GetComponentID<Component>()] = instance;
    auto activeComponentsBefore = activeComponents;
#ifdef SCRIPTING_ENABLED
    auto activeScriptComponentsBefore = activeScriptComponents;
#endif
    world->createActions.emplace_back([this, activeComponentsBefore
#ifdef SCRIPTING_ENABLED
    , activeScriptComponentsBefore
#endif
    ]() {
    
        auto& systemsUsingComponent = world->componentSystems[IDHelper::GetComponentID<Component>()];
        for(auto system : systemsUsingComponent) {
            auto& bitSet = world->systemBitsets[system->index];
            bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
            if (isInterest) {
                systemIndices[system->index]=(int)system->objects.size();
                system->objects.push_back(this);
                system->ObjectAdded(this);
            }
        }

#ifdef SCRIPTING_ENABLED
        CheckForScriptSystemsAddition(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
#endif
    });
}



template<typename... ComponentList>
void GameSystem<ComponentList...>::Initialize(GameWorld *world) {
    std::tuple<ComponentList*...> componentsTuple;
    Meta::for_each_in_tuple_non_const(componentsTuple, [world](auto c) {
        auto& components = world->components;
        using ComponentType = std::remove_pointer_t<decltype(c)>;
        int componentID = IDHelper::GetComponentID<ComponentType>();
        if (componentID>=components.size()) {
            components.resize(componentID + 1, 0);
        }
        if (!components[componentID]) {
            components[componentID] = new Container<ComponentType>;
        }
    });
}











