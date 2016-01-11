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
#include <type_traits>
#include <assert.h>

#define FWD(x) (::std::forward<decltype(x)>(x))

class GameObject;
struct GameWorldSettings;

class GameWorld {
private:
    friend class GameObject;
    
    using Settings = GameWorldSettings;
    using Objects = Container<GameObject>;
    
    using Systems = typename Settings::SystemsTuple;
    using UpdateSystems = typename Settings::UpdateSystemsTuple;
    using AddedToWorldSystems = typename Settings::AddedToWorldSystemsTuple;
    
    using ComponentSystems = typename Settings::ComponentSystemsTuple;
    
    using SystemBitsets = typename Settings::SystemBitsets;
    using Components = typename Settings::AllComponents;
    
    using Worlds = std::vector<GameWorld*>;
    
    using Actions = std::vector<std::function<void()>>;
    
    Objects objects;
    
    Systems systems;
    UpdateSystems updateSystems;
    AddedToWorldSystems addedToWorldSystems;
    ComponentSystems componentSystems;
    
    SystemBitsets systemBitsets;
    Components components;
    
    Actions createActions;
    Actions removeActions;
    
    void InitializeSystemBitsets();
public:

    GameWorld();
    void Initialize();
    GameObject* CreateObject();
    
    template<typename System>
    System& GetSystem() {
        return std::get<Settings::template GetSystemID<System>()>(systems);
    }
    
    void Update(float dt);
    void DoActions(Actions& list);
    int ObjectCount();
};

class GameObject {
private:
    GameObject()  { }
    
    using Settings = GameWorldSettings;
    
    typename Settings::Bitset activeComponents;
    typename Settings::Bitset removedComponents;
    
    std::array<void*, typename Settings::UniqueComponents{}.size()> components;
    std::array<int, typename Settings::UniqueComponents{}.size()> componentIndices;
    std::array<int, typename Settings::Systems{}.size()> indexInSystem;
    
    bool isRemoved;
    Container<GameObject>::ObjectInstance* instance;
    GameWorld* world;
    
    friend class GameWorld;
    friend class Container<GameObject>::ObjectInstance;
    
    void Reset();
    
public:
    
    void Remove();

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
            typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)components[Settings::template GetComponentID<Component>()];
            
            
            auto activeComponentsBefore = activeComponents;
            activeComponents[Settings::template GetComponentID<Component>()] = false;
            removedComponents[Settings::template GetComponentID<Component>()] = false;
            meta::for_each_in_tuple(std::get< Settings::template GetComponentID<Component>() >(world->componentSystems), [activeComponentsBefore, instance, this] (auto systemPointer) {
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
            
        });
    }
    
    template<typename Component, typename... TArgs>
    auto AddComponent(TArgs&&... mXs) {
        assert(!HasComponent<Component>());
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world->components);
        SetComponent<Component>(container.CreateObject());
        return GetComponent<Component>();
    }
    
    template<typename Component>
    auto AddComponent(GameObject* source) {
        assert(source);
        assert(!HasComponent<Component>());
        assert(source->HasComponent<Component>());
        typename Container<Component>::ObjectInstance* instance = (typename Container<Component>::ObjectInstance*)source->components[Settings::template GetComponentID<Component>()];
        ++instance->references;
        SetComponent<Component>(instance);
        return GetComponent<Component>();
    }
    
private:
    template<typename Component>
    void SetComponent(typename Container<Component>::ObjectInstance* instance) {
        activeComponents[Settings::template GetComponentID<Component>()] = true;
        components[Settings::template GetComponentID<Component>()] = instance;
        auto activeComponentsBefore = activeComponents;
        world->createActions.emplace_back([activeComponentsBefore, this]() {
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
        });
    }
};



