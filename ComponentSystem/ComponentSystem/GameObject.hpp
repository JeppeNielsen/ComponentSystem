//
//  GameObject.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "IDHelper.hpp"
#include "TypeInfo.hpp"
#include "Container.hpp"

class GameWorld;

#ifdef SCRIPTING_ENABLED

struct IGameObject {
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0;
    virtual void* AddScriptComponent(int componentID) = 0;
    virtual void RemoveScriptComponent(int componentID) = 0;
};

#endif

class GameObject
#ifdef SCRIPTING_ENABLED
: public IGameObject
#endif
{
public:
    using ObjectCollection = std::vector<GameObject*>;

private:
    friend class GameWorld;
    
    using Bitset = std::bitset<64>;
    
    Bitset activeComponents;
    Bitset removedComponents;
    
    using SystemIndices = int*;
    
    SystemIndices systemIndices;
    
    bool isRemoved;
    Container<GameObject>::ObjectInstance* instance;
    GameWorld* world;
    
    friend class Container<GameObject>::ObjectInstance;
    
    void Reset();
    void SetWorld(GameWorld* w);
    GameObject(const GameObject& other) = default;

protected:
    
    friend class IGameWorld;

    using ComponentPtr = void*;
    ComponentPtr* components;

    GameObject();
    virtual ~GameObject();
    
    ObjectCollection children;
public:

    Property<GameObject*> Parent;
    const ObjectCollection& Children() { return children; }
    Property<int> Order;
    
    template<typename T>
    T* GetComponent() {
        return (T*)components[IDHelper::GetComponentID<T>()];
    }
    
    void Remove();
    
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[IDHelper::GetComponentID<Component>()];
    }
    
    template<typename Component>
    void RemoveComponent();
    
    template<typename Component>
    Component* AddComponent();
    
    template<typename Component>
    Component* AddComponent(GameObject* source);
    /*
    using SerializePredicate = std::function<bool(GameObject*, int)>;
    
    void ToJson(std::ostream& stream, SerializePredicate predicate) {
        minijson::writer_configuration config;
        config = config.pretty_printing(true);
        minijson::object_writer writer(stream, config);
        WriteJson(writer, predicate);
        writer.close();
    }
    */
    
    /*
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
    */
    
private:
    template<typename Component>
    void SetComponent(typename Container<Component>::ObjectInstance* instance);
    
    /*
    void WriteJson(minijson::object_writer& writer, SerializePredicate predicate) {

        minijson::object_writer gameObject = writer.nested_object("GameObject");
        minijson::array_writer components = gameObject.nested_array("Components");
        
        meta::for_each_in_tuple(world->serializableComponents, [this, &components, &predicate] (auto componentPointer) {
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (HasComponent<ComponentType>()) {
                if (!(predicate && !predicate(this, IDHelper::GetComponentID<ComponentType>()))) {
                    SerializeComponent<ComponentType>(components, false, 0);
                }
            }
        });

        components.close();
        
        if (!children.empty()) {
            minijson::array_writer children_object = gameObject.nested_array("Children");
            for(auto child : children) {
                if (predicate && !predicate(child, -1)) {
                    continue;
                }
                GameObject* childSpecific = (GameObject*)child;
                minijson::object_writer child_object = children_object.nested_object();
                childSpecific->WriteJson(child_object, predicate);
                child_object.close();
            }
            children_object.close();
        }
        
        gameObject.close();
    }
    
    template<typename Component>
    void SerializeComponent(minijson::array_writer& writer, bool isReference, std::string* referenceID ) {
        minijson::object_writer componentWriter = writer.nested_object();
        Component* component = GetComponent<Component>();
        auto type = component->GetType();
        std::string& name = world->componentNames[Settings::template GetComponentID<Component>()];
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
            if (componentName == world->componentNames[Settings::template GetComponentID<ComponentType>()]) {
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
    */
    
    template<typename Component>
    TypeInfo GetComponentTypeInfo() {
        Component* component = GetComponent<Component>();
        return component->GetType();
    }
    
#ifdef SCRIPTING_ENABLED
    //Scripting *******************************
    
    // Scripting Data
    using ScriptComponent = Container<void*>::ObjectInstance*;
    ScriptComponent* scriptComponents;
    
    int* scriptSystemIndices;
    
    typename GameWorld::ScriptBitset activeScriptComponents;
    typename GameWorld::ScriptBitset removedScriptComponents;

    void* GetComponent(int componentID) override {
        //&((typename Container<Component>::ObjectInstance*)components[componentID])->object;
        return components[componentID]; // this works since "object" is first in ObjectInstance;
    }
    private:
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
public:
    
    void* GetScriptComponent(int componentID) override {
        if (!activeScriptComponents[componentID]) return 0;
        typename Container<void*>::ObjectInstance* instance = (typename Container<void*>::ObjectInstance*)scriptComponents[componentID];
        return instance->object;
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
    void CheckForScriptSystemsAddition(const std::vector<short>& systems, const typename Settings::Bitset& activeComponentsBefore, const typename GameWorld::ScriptBitset& activeScriptComponentsBefore) {
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
    
    void CheckForScriptSystemsRemoval(const std::vector<short>& systems, const typename Settings::Bitset& activeComponentsBefore, const typename GameWorld::ScriptBitset& activeScriptComponentsBefore) {
    
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
            auto* last = system->RemoveObject(index);
            GameObject* lastGameObject = (GameObject*)last;
            lastGameObject->scriptSystemIndices[systemIndex] = index;
        }
    }
#endif
};