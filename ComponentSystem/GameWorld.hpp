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

struct GameWorld {
public:
    using Settings = GameWorldSettings;
    using Objects = Container<GameObject>;
    
    using Systems = typename Settings::SystemsTuple;
    using UpdateSystems = typename Settings::UpdateSystemsTuple;
    
    using ComponentSystems = typename Settings::ComponentSystemsTuple;
    
    using SystemBitsets = typename Settings::SystemBitsets;
    using Components = typename Settings::AllComponents;
    
    using Worlds = std::vector<GameWorld*>;
    
    using Actions = std::vector<std::function<void()>>;
    
    Objects objects;
    
    Systems systems;
    UpdateSystems updateSystems;
    ComponentSystems componentSystems;
    
    SystemBitsets systemBitsets;
    Components components;
    
    int worldIndex;
    static Worlds worlds;
    
    Actions createActions;
    Actions removeActions;
    
    void InitializeSystemBitsets() {
        int i = 0;
        meta::for_each_in_tuple(systems, [&](auto system) {
            meta::for_each_in_tuple(meta::Iterator<typename decltype(system)::Components>{}.Iterate(), [&,this](auto component) {
                systemBitsets[i][Settings::template GetComponentID<decltype(component)>()] = true;
            });
            ++i;
        });
    }
    
    
    
    friend class GameObject;
    
public:

    GameWorld() {
        InitializeSystemBitsets();
        worldIndex = (int)worlds.size();
        worlds.push_back(this);
    }
    
    ~GameWorld() {
        worlds[worldIndex] = 0;
    }

    Handle<GameObject> CreateObject();
    
    template<typename System>
    System& GetSystem() {
        return std::get<Settings::template GetSystemID<System>()>(systems);
    }
    
    void Update(float dt) {
        DoActions(createActions);
        DoActions(removeActions);
        meta::for_each_in_tuple(updateSystems, [this, dt] (auto& system) {
            std::get<
                    Settings::template GetSystemID<
                    std::remove_const_t< std::remove_reference_t<decltype(system)> >
                    >()
                    >(systems).Update(dt);
        });
    }
    
    void DoActions(Actions& list) {
       for(int i=0; i<list.size(); ++i) {
            list[i]();
       }
       list.clear();
    }
    
};



class GameObject {
private:
    GameObject() : isRemoved(false), worldIndex(0), activeComponents(0), removedComponents(0) { }
    
    using Settings = GameWorldSettings;
    
    typename Settings::Bitset activeComponents;
    typename Settings::Bitset removedComponents;
    
    std::array<int, typename Settings::UniqueComponents{}.size()> components;
    std::array<int, typename Settings::Systems{}.size()> indexInSystem;
    
    int worldIndex;
    bool isRemoved;
    int index;
    
    friend class GameWorld;
    GameWorld& GetWorld() const {
        return *GameWorld::worlds[worldIndex];
    }
    
    friend class GameWorld;
    friend class std::allocator<GameObject>;
    
public:
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[Settings::template GetComponentID<Component>()];
    }
    
    template<typename Component>
    auto GetComponent() {
        if (!HasComponent<Component>()) {
            return Handle<Component>();
        }
        auto& world = GetWorld();
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world.components);
        return container.GetHandle(components[Settings::template GetComponentID<Component>()]);
    }
    
    template<typename Component>
    void RemoveComponent() {
        if (removedComponents[Settings::template GetComponentID<Component>()]) return;
        assert(HasComponent<Component>());
        removedComponents[Settings::template GetComponentID<Component>()] = true;
        auto& world = GetWorld();
        int thisIndex = this->index;
        
        world.removeActions.emplace_back([&world, thisIndex]() {
            auto* object = world.objects.GetObject(thisIndex);
            auto& container = std::get<Settings::template GetComponentID<Component>()>(world.components);
            container.RemoveObject(object->components[Settings::template GetComponentID<Component>()]);
            auto activeComponentsBefore = object->activeComponents;
            object->activeComponents[Settings::template GetComponentID<Component>()] = false;
            object->removedComponents[Settings::template GetComponentID<Component>()] = false;
            meta::for_each_in_tuple_non_const(std::get< Settings::template GetComponentID<Component>() >(world.componentSystems), [&world, activeComponentsBefore, object] (auto& sys) {
                auto& system = std::get< Settings::template GetSystemID< std::remove_reference_t<decltype(sys)> >() >(world.systems);
                auto& bitSet = world.systemBitsets[Settings::template GetSystemID<std::remove_reference_t<decltype(system)> >()];
                bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (wasInterest) {
                    int objectIndexInSystem = object->indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                    int lastIndex = (int)system.objects.size() - 1;
                    GameObject& lastObject = *system.objects[lastIndex].operator->();
                    lastObject.indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=objectIndexInSystem;
                    std::swap(system.objects[objectIndexInSystem], system.objects[lastIndex]);
                    system.objects.pop_back();
                    system.ObjectRemoved(world.objects.GetHandle(object->index));
                }
            });
        });
    }
    
    template<typename Component, typename... TArgs>
    auto AddComponent(TArgs&&... mXs) {
        assert(!HasComponent<Component>());
        
        activeComponents[Settings::template GetComponentID<Component>()] = true;
        
        auto& world = GetWorld();
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world.components);
        int index = container.CreateObject();
        components[Settings::template GetComponentID<Component>()] = index;
        auto activeComponentsBefore = activeComponents;
        int thisIndex = this->index;
        
        world.createActions.emplace_back([&world, activeComponentsBefore, thisIndex]() {
            meta::for_each_in_tuple_non_const(std::get< Settings::template GetComponentID<Component>() >(world.componentSystems), [&world, activeComponentsBefore, thisIndex] (auto& sys) {
                auto& system = std::get< Settings::template GetSystemID< std::remove_reference_t<decltype(sys)> >() >(world.systems);
                auto& bitSet = world.systemBitsets[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()];
                bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (isInterest) {
                    auto handle = world.objects.GetHandle(thisIndex);
                    handle->indexInSystem[Settings::template GetSystemID< std::remove_reference_t<decltype(system)> >()]=(int)system.objects.size();
                    system.objects.push_back(handle);
                    system.ObjectAdded(handle);
                }
            });
        });
        
        return container.GetHandle(index);
    }
    
    void Remove() {
        if (isRemoved) return;
        isRemoved = true;
        auto& world = GetWorld();
    
        meta::for_each_in_tuple(world.components, [this] (auto& component) {
            RemoveComponent<meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>>();
        });
        
        world.removeActions.emplace_back([&world, this]() {
            world.objects.RemoveObject(index);
            isRemoved = false;
        });
    }
};



