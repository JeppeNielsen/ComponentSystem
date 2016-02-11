//
//  GameWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSettings.hpp"
#include "Container.hpp"
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include <type_traits>
#include <assert.h>
#include "Property.hpp"
#include "IScriptSystem.hpp"

#define FWD(x) (::std::forward<decltype(x)>(x))

class GameObject;
struct GameWorldSettings;

class GameWorld {
private:
    friend class GameObject;
    
    using Settings = GameWorldSettings;
    using Objects = Container<GameObject>;
    
    using Systems = typename Settings::SystemsTuple;
    using InitializeSystems = typename Settings::InitializeSystems;
    using UpdateSystems = typename Settings::UpdateSystems;
    using RenderSystems = typename Settings::RenderSystems;
    
    using ComponentSystems = typename Settings::ComponentSystemsTuple;
    
    using SystemBitsets = typename Settings::SystemBitsets;
    using Components = typename Settings::AllComponents;
    using SerializableComponents = typename Settings::SerializableComponents;
    using ComponentNames = typename Settings::ComponentNames;
    
    using Actions = std::vector<std::function<void()>>;
    
    Objects objects;
    
    Systems systems;
    
    InitializeSystems initializeSystems;
    UpdateSystems updateSystems;
    RenderSystems renderSystems;
    
    ComponentSystems componentSystems;
    
    SystemBitsets systemBitsets;
    Components components;
    
    SerializableComponents serializableComponents;
    ComponentNames componentNames;
    
    Actions createActions;
    Actions removeActions;
    
    void InitializeSystemBitsets() {
        meta::for_each_in_tuple(systems, [&](auto& system) {
            using SystemType = std::remove_const_t<std::remove_reference_t<decltype(system)>>;
            meta::for_each_in_tuple(meta::IteratorPointer<typename SystemType::Components>{}.Iterate(), [&,this](auto component) {
                systemBitsets[Settings::template GetSystemID<SystemType>()]
                             [Settings::template GetComponentID<std::remove_pointer_t< decltype(component) >>()] = true;
            });
        });
    }
    
    void InitializeComponentNames() {
        meta::for_each_in_tuple(serializableComponents, [this] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            componentNames[Settings::GetComponentID<ComponentType>()] = ComponentType{}.GetType().name;
        });
    }
public:

    template<typename System>
    System& GetSystem() {
        return std::get<Settings::template GetSystemID<System>()>(systems);
    }

    template<typename T>
    GameObject* CreateObject();
    
    template<typename T>
    GameObject* CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated);
    
    GameWorld() {
        InitializeSystemBitsets();
        InitializeComponentNames();
    }

    void Initialize() {
        meta::for_each_in_tuple(initializeSystems, [&](auto system) {
            std::get<
                Settings::template GetSystemID<
                    std::remove_pointer_t<decltype(system)>
                >()
            >(systems).Initialize(*this);
        });
    }

    void Update(float dt) {
        DoActions(createActions);
        DoActions(removeActions);
        meta::for_each_in_tuple(updateSystems, [this, dt] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Update(dt);
        });
        
        for(auto scriptSystem : scriptSystems) {
            scriptSystem->Update(dt);
        }
    }
    
    void Render() {
        meta::for_each_in_tuple(renderSystems, [this] (auto system) {
            std::get<
                    Settings::template GetSystemID<
                        std::remove_pointer_t< decltype(system)>
                    >()
                >(systems).Render();
        });
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

    template<typename T>
    void Clear();
    
    ~GameWorld() {
        Clear<void>();
    }
    
    void EnumerateComponentClasses(std::function<void(std::string, int)> callback) {
        meta::for_each_in_tuple(serializableComponents, [this, callback] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            callback(componentNames[Settings::GetComponentID<ComponentType>()], Settings::GetComponentID<ComponentType>());
        });
    }
    
    //Scripting
    using StaticScriptSystemComponents = std::array<std::vector<short>, Settings::UniqueComponents{}.size()>;
    StaticScriptSystemComponents staticScriptSystemComponents;
    using ScriptSystemComponents = std::vector<std::vector<short>>;
    ScriptSystemComponents dynamicScriptSystemComponents;
    
    using ScriptComponents = std::vector<Container<void*>>;
    ScriptComponents scriptComponents;
    
    using ScriptBitset = std::vector<bool>;
    
    struct ScriptSystemData {
        Settings::Bitset staticComponents;
        std::vector<short> scriptComponents;
    };
    
    using ScriptSystems = std::vector<IScriptSystem*>;
    ScriptSystems scriptSystems;
    
    using ScriptSystemsData = std::vector<ScriptSystemData>;
    ScriptSystemsData scriptSystemsData;
    
    template<typename T>
    void ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted);
    
    template<typename T>
    void InitializeScriptData(
            int numSystems, int numComponents,
            std::function<IScriptSystem*(int)> onSystemCreate,
            std::function<void(Container<void*>&, int)> onComponentCreate,
            std::function<void(
                        StaticScriptSystemComponents& staticScriptSystemComponents,
                        ScriptSystemComponents& dynamicScriptSystemComponents,
                        ScriptSystemsData& scriptSystemsData)> onSystemData
            );
};

struct IGameObject {
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0;
    virtual void* AddScriptComponent(int componentID) = 0;
    virtual void RemoveScriptComponent(int componentID) = 0;
};

class GameObject : public IGameObject {
private:
    
    using Settings = GameWorldSettings;
    using ObjectCollection = std::vector<GameObject*>;
    
    typename Settings::Bitset activeComponents;
    typename Settings::Bitset removedComponents;
    
    std::array<void*, typename Settings::UniqueComponents{}.size()> components;
    std::array<int, typename Settings::Systems{}.size()> indexInSystem;
    
    bool isRemoved;
    Container<GameObject>::ObjectInstance* instance;
    GameWorld* world;
    ObjectCollection children;
    
    
    
    friend class GameWorld;
    friend class Container<GameObject>::ObjectInstance;
    
    void Reset() {
        isRemoved = false;
        activeComponents.reset();
        removedComponents.reset();
        
        for(int i=0; i<activeScriptComponents.size(); ++i) {
            activeScriptComponents[i]=false;
            removedScriptComponents[i]=false;
        }
    }
    
    GameObject() {
        Parent = 0;
        Parent.Changed.Bind([this](GameObject* value) {
            assert(value!=this);
            GameObject* prevParent = Parent.PreviousValue();
            if (prevParent) {
                auto& children = prevParent->children;
                children.erase(std::find(children.begin(), children.end(), this));
            }
            
            if (value) {
                value->children.push_back(this);
            }
        });
        world = 0;
    }
    
    void SetWorld(GameWorld* w) {
        if (world) return;
        world = w;
        InitializeScriptingData();
    }
    
public:

    Property<GameObject*> Parent;
    const ObjectCollection& Children() { return children; }
    
    void Remove() {
        if (isRemoved) return;
        isRemoved = true;
        
        meta::for_each_in_tuple(world->components, [this] (auto& component) {
            using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
            if (HasComponent<ComponentType>()) {
                RemoveComponent<ComponentType>();
            }
        });
        
        world->removeActions.emplace_back([this]() {
            world->objects.RemoveObject(instance);
            isRemoved = false;
        });
        
        for(auto child : children) {
            child->Remove();
        }
    }
    
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[Settings::template GetComponentID<Component>()];
    }
    
    template<typename Component>
    Component* GetComponent() {
        if (!HasComponent<Component>()) {
            return 0;
        }
        return &((typename Container<Component>::ObjectInstance*)components[Settings::template GetComponentID<Component>()])->object;
    }
    
    template<typename Component>
    void RemoveComponent() {
        if (removedComponents[Settings::template GetComponentID<Component>()]) return;
        assert(HasComponent<Component>());
        removedComponents[Settings::template GetComponentID<Component>()] = true;
        
        world->removeActions.emplace_back([this]() {
            auto activeComponentsBefore = activeComponents;
            auto activeScriptComponentsBefore = activeScriptComponents;
            activeComponents[Settings::template GetComponentID<Component>()] = false;
            removedComponents[Settings::template GetComponentID<Component>()] = false;
            meta::for_each_in_tuple(std::get< Settings::template GetComponentID<Component>() >(world->componentSystems), [this, activeComponentsBefore, activeScriptComponentsBefore] (auto systemPointer) {
                auto& system = std::get< Settings::template GetSystemID< std::remove_pointer_t<decltype(systemPointer)> >() >(world->systems);
                auto& bitSet = world->systemBitsets[Settings::template GetSystemID<std::remove_reference_t<decltype(system)> >()];
                bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (wasInterest) {
                    system.ObjectRemoved(this);
                    int objectIndexInSystem = indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                    int lastIndex = (int)system.objects.size() - 1;
                    GameObject* lastObject = system.objects[lastIndex];
                    lastObject->indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=objectIndexInSystem;
                    system.objects[objectIndexInSystem]=lastObject;
                    system.objects.pop_back();
                }
            });
            
            CheckForScriptSystemsRemoval(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
            
            typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[Settings::template GetComponentID<Component>()];
            auto& container = std::get<Settings::template GetComponentID<Component>()>(world->components);
            container.RemoveObject(instance);
        });
    }
    
    template<typename Component, typename... TArgs>
    Component* AddComponent(TArgs&&... mXs) {
        assert(!HasComponent<Component>());
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world->components);
        SetComponent<Component>(container.CreateObject());
        return GetComponent<Component>();
    }
    
    template<typename Component>
    Component* AddComponent(GameObject* source) {
        assert(source);
        assert(!HasComponent<Component>());
        assert(source->HasComponent<Component>());
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)source->components[Settings::template GetComponentID<Component>()];
        ++instance->references;
        SetComponent<Component>(instance);
        return GetComponent<Component>();
    }
    
    using SerializePredicate = std::function<bool(GameObject*, size_t)>;
    
    void ToJson(std::ostream& stream, SerializePredicate predicate) {
        minijson::writer_configuration config;
        config = config.pretty_printing(true);
        minijson::object_writer writer(stream, config);
        WriteJson(writer, predicate);
        writer.close();
    }
    
    std::vector<TypeInfo> GetComponentTypeInfos() {
        std::vector<TypeInfo> infos;
        meta::for_each_in_tuple(world->serializableComponents, [this, &infos] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (HasComponent<ComponentType>()) {
                infos.push_back(GetComponentTypeInfo<ComponentType>());
            }
        });
        return infos;
    }
    
private:
    template<typename Component>
    void SetComponent(typename Container<Component>::ObjectInstance* instance) {
        activeComponents[Settings::template GetComponentID<Component>()] = true;
        components[Settings::template GetComponentID<Component>()] = instance;
        auto activeComponentsBefore = activeComponents;
        auto activeScriptComponentsBefore = activeScriptComponents;
        world->createActions.emplace_back([this, activeComponentsBefore, activeScriptComponentsBefore]() {
            meta::for_each_in_tuple(std::get< Settings::template GetComponentID<Component>() >(world->componentSystems), [this, activeComponentsBefore] (auto systemPointer) {
                auto& system = std::get< Settings::template GetSystemID< std::remove_pointer_t<decltype(systemPointer)> >() >(world->systems);
                auto& bitSet = world->systemBitsets[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (isInterest) {
                    indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=(int)system.objects.size();
                    system.objects.push_back(this);
                    system.ObjectAdded(this);
                }
            });
            
            CheckForScriptSystemsAddition(world->staticScriptSystemComponents[Settings::template GetComponentID<Component>()], activeComponentsBefore, activeScriptComponentsBefore);
        });
    }
    
    void WriteJson(minijson::object_writer& writer, SerializePredicate predicate) {

        minijson::object_writer gameObject = writer.nested_object("GameObject");
        minijson::array_writer components = gameObject.nested_array("Components");
        
        meta::for_each_in_tuple(world->serializableComponents, [this, &components, &predicate] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (HasComponent<ComponentType>()) {
                if (!(predicate && !predicate(this, Settings::GetComponentID<ComponentType>()))) {
                    SerializeComponent<ComponentType>(components, false, 0);
                }
            }
        });

        components.close();
        /*
        if (!children.empty()) {
            minijson::array_writer children_object = gameObject.nested_array("Children");
            for(auto child : children) {
                if (predicate && !predicate(child, -1)) {
                    continue;
                }
                minijson::object_writer child_object = children_object.nested_object();
                child->WriteJson(child_object, predicate);
                child_object.close();
            }
            children_object.close();
        }
        */
        gameObject.close();
    }
    
    template<typename Component>
    void SerializeComponent(minijson::array_writer& writer, bool isReference, std::string* referenceID ) {
        minijson::object_writer componentWriter = writer.nested_object();
        Component* component = GetComponent<Component>();
        auto type = component->GetType();
        std::string& name = world->componentNames[Settings::GetComponentID<Component>()];
        if (!isReference) {
            minijson::object_writer jsonComponent = componentWriter.nested_object(name.c_str());
            type.Serialize(jsonComponent);
            jsonComponent.close();
        } else {
            std::string referenceName = name + ":ref";
            minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
            if (!referenceID) {
                jsonComponent.write("id", "");
            } else {
                jsonComponent.write("id", *referenceID);
            }
            jsonComponent.close();
        }
        componentWriter.close();
    }
    
    void AddComponent(minijson::istream_context& context, std::string componentName) {
        bool addedAny = false;
        meta::for_each_in_tuple(world->serializableComponents, [this, &context, &componentName, &addedAny] (auto componentPointer) {
            if (addedAny) {
                return;
            }
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (componentName == world->componentNames[Settings::GetComponentID<ComponentType>()]) {
                if (!HasComponent<ComponentType>()) {
                    ComponentType* component = AddComponent<ComponentType>();
                    auto type = component->GetType();
                    type.Deserialize(context);
                    addedAny = true;
                } else {
                    std::cout<<"Only one component per type allowed per object"<<std::endl;
                    minijson::ignore(context);
                }
            }
        });
        if (!addedAny) {
            minijson::ignore(context);
        }
    }
    
    template<typename Component>
    TypeInfo GetComponentTypeInfo() {
        Component* component = GetComponent<Component>();
        return component->GetType();
    }
    
    //Scripting *******************************
    
    // Scripting Data
    using ScriptComponent = Container<void*>::ObjectInstance*;
    ScriptComponent* scriptComponents;
    
    int* scriptSystemIndices;
    
    GameWorld::ScriptBitset activeScriptComponents;
    GameWorld::ScriptBitset removedScriptComponents;
    // /ScriptingData
    
    void* GetComponent(int componentID) override {
        //&((typename Container<Component>::ObjectInstance*)components[componentID])->object;
        return components[componentID]; // this works since "object" is first in ObjectInstance;
    }
    
    void* AddComponent(int componentID) override {
        int counter = 0;
        
        meta::for_each_in_tuple(world->components, [this, &counter, componentID] (auto& component) {
            if (counter == componentID) {
                using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
                AddComponent<ComponentType>();
            }
            counter++;
        });
        return GetComponent(componentID);
    }
    
    void RemoveComponent(int componentID) override {
        int counter = 0;
        meta::for_each_in_tuple(world->components, [this, &counter, componentID] (auto& component) {
            if (counter == componentID) {
                using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
                RemoveComponent<ComponentType>();
            }
            counter++;
        });
    }
    
    void* GetScriptComponent(int componentID) override {
        if (!activeScriptComponents[componentID]) return 0;
        typename Container<void*>::ObjectInstance* instance = (typename Container<void*>::ObjectInstance*)scriptComponents[componentID];
        return instance->object;
    }
    
public:

    void ClearScriptingData() {
        delete[] scriptComponents;
        delete[] scriptSystemIndices;
        activeScriptComponents.clear();
        removedScriptComponents.clear();
    }
    
    void InitializeScriptingData() {
        std::size_t numScriptComponents = world->scriptComponents.size();
        scriptComponents = new ScriptComponent[numScriptComponents];
        for (int i=0; i<numScriptComponents; i++) {
            scriptComponents[i]=0;
        }
        scriptSystemIndices = new int[world->scriptSystems.size()];
        activeScriptComponents.resize(numScriptComponents);
        removedScriptComponents.resize(numScriptComponents);
    }

    void* AddScriptComponent(int componentID) override {
        if (activeScriptComponents[componentID]) {
            return scriptComponents[componentID];
        }
        
        activeScriptComponents[componentID] = true;
        scriptComponents[componentID] = world->scriptComponents[componentID].CreateObject();
        
        auto activeComponentsBefore = activeComponents;
        auto activeScriptComponentsBefore = activeScriptComponents;
        world->createActions.emplace_back([this, componentID, activeComponentsBefore, activeScriptComponentsBefore]() {
            CheckForScriptSystemsAddition(world->dynamicScriptSystemComponents[componentID], activeComponentsBefore, activeScriptComponentsBefore);
        });
        
        return scriptComponents[componentID];
    }

    void RemoveScriptComponent(int componentID) override {
        if (!activeScriptComponents[componentID]) return;
        
        if (removedScriptComponents[componentID]) return;
        removedScriptComponents[componentID] = true;
        
        world->removeActions.emplace_back([this, componentID]() {
            
            auto activeScriptComponentsBefore = activeScriptComponents;
            
            activeScriptComponents[componentID] = false;
            removedScriptComponents[componentID] = false;
            
            CheckForScriptSystemsRemoval(world->dynamicScriptSystemComponents[componentID], activeComponents, activeScriptComponentsBefore);
            
            typename Container<void*>::ObjectInstance* instance = (typename Container<void*>::ObjectInstance*)scriptComponents[componentID];
            world->scriptComponents[componentID].RemoveObject(instance);
        });
    }

private:
    void CheckForScriptSystemsAddition(const std::vector<short>& systems, const Settings::Bitset& activeComponentsBefore, const GameWorld::ScriptBitset& activeScriptComponentsBefore) {
        for(int i = 0; i<systems.size(); ++i) {
            short systemIndex = systems[i];
            IScriptSystem* system = world->scriptSystems[systemIndex];
            auto& systemData = world->scriptSystemsData[systemIndex];
            if (systemData.staticComponents!=0 && !((activeComponentsBefore & systemData.staticComponents) == systemData.staticComponents)) {
                continue;
            }
            std::size_t componentCount = systemData.scriptComponents.size();
            bool match = true;
            for(int j=0; j<componentCount; ++j) {
                if (!activeScriptComponentsBefore[systemData.scriptComponents[j]]) {
                    match = false;
                    break;
                }
            }
            if (!match) continue;
            scriptSystemIndices[systemIndex] = system->AddObject(this);
            system->ObjectAdded(this);
        }
    }
    
    void CheckForScriptSystemsRemoval(const std::vector<short>& systems, const Settings::Bitset& activeComponentsBefore, const GameWorld::ScriptBitset& activeScriptComponentsBefore) {
    
        for(int i = 0; i<systems.size(); ++i) {
            short systemIndex = systems[i];
            IScriptSystem* system = world->scriptSystems[systemIndex];
            auto& systemData = world->scriptSystemsData[systemIndex];
            if (systemData.staticComponents!=0 && !((activeComponentsBefore & systemData.staticComponents) == systemData.staticComponents)) {
                continue;
            }
            std::size_t componentCount = systemData.scriptComponents.size();
            bool match = true;
            for(int j=0; j<componentCount; ++j) {
                if (!activeScriptComponentsBefore[systemData.scriptComponents[j]]) {
                    match = false;
                    break;
                }
            }
            if (!match) continue;
            system->ObjectRemoved(this);
            int index = scriptSystemIndices[systemIndex];
            GameObject* last = system->RemoveObject(index);
            last->scriptSystemIndices[systemIndex] = index;
        }
    }
    
};

template<typename T = void>
GameObject* GameWorld::CreateObject() {
    auto object = objects.CreateObject();
    object->object.instance = object;
    object->object.SetWorld(this);
    object->object.Reset();
    return &object->object;
}

template<typename T = void>
void GameWorld::Clear() {
    for(int i=0; i<objects.Size(); ++i) {
        objects.Get(i)->Remove();
    }
    DoActions(createActions);
    DoActions(removeActions);
}

template<typename T = void>
GameObject* GameWorld::CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) {
    minijson::istream_context context(jsonStream);
    GameObject* object = 0;
    try {
         minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string name = n;
            if (name == "GameObject" && v.type() == minijson::Object) {
                object = CreateObject();
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
                    } /*else if (name == "Children" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            GameObject* child = CreateGameObjectJson(context, iterator);
                            if (child) {
                                child->Parent = object;
                            }
                        });
                    }
                    */
                    if (onCreated) {
                        onCreated(object);
                    }
                });
            }
         });
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    return object;
}

template<typename T = void>
void GameWorld::ClearScripingData(std::function<void(IScriptSystem*)> onSystemDeleted) {
    for(int i=0; i<ObjectCount(); ++i) {
        GetObject(i)->ClearScriptingData();
    }

    for(int i=0; i<Settings::UniqueComponents{}.size(); ++i) {
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

template<typename T = void>
void GameWorld::InitializeScriptData(int numSystems, int numComponents,
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
/*
template<typename T = void>
void GameWorld::InitializeScripingData(std::function<void(IScriptSystem*)> onSystemDeleted) {
    for(int i=0; i<ObjectCount(); ++i) {
        GetObject(i)->InitializeScriptingData();
    }
}
*/

