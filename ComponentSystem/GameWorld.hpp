//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSettings.hpp"
#include "GameObject.hpp"
#include "GameSystem.hpp"
#include "Container.hpp"
#include <type_traits>
#include <assert.h>

#define FWD(x) (::std::forward<decltype(x)>(x))

template<typename Settings>
struct GameObject;



template<typename Settings>
struct GameWorld {
public:
    using Objects = Container<GameObject<Settings>>;
    
    using Systems = typename Settings::SystemsTuple;
    using UpdateSystems = typename Settings::UpdateSystemsTuple;
    
    using ComponentSystems = typename Settings::ComponentSystemsTuple;
    
    using SystemBitsets = typename Settings::SystemBitsets;
    using Components = typename Settings::AllComponents;
    
    using Worlds = std::vector<GameWorld<Settings>*>;
    
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
    
    
    
    friend class GameObject<Settings>;
    
public:

    GameWorld() {
        InitializeSystemBitsets();
        worldIndex = (int)worlds.size();
        worlds.push_back(this);
    }
    
    ~GameWorld() {
        worlds[worldIndex] = 0;
    }

    auto CreateObject() {
        int index = objects.CreateObject();
        objects.GetObject(index)->index = index;
        return objects.GetHandle(index);
    }
    
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

template<typename Settings>
typename GameWorld<Settings>::Worlds GameWorld<Settings>::worlds;


template<typename Settings>
class GameObject {
private:
    GameObject() : isRemoved(false), worldIndex(0) { }

    typename Settings::Bitset activeComponents;
    std::array<int, typename Settings::UniqueComponents{}.size()> components;
    int worldIndex;
    bool isRemoved;
    int index;
    
    friend class GameWorld<Settings>;
    GameWorld<Settings>& GetWorld() const {
        return *GameWorld<Settings>::worlds[worldIndex];
    }
    
    friend class GameWorld<Settings>;
    friend class std::allocator<GameObject<Settings>>;
    
public:
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[Settings::template GetComponentID<Component>()];
    }
    
    template<typename Component>
    auto GetComponent() {
        if (!HasComponent<Component>()) {
            return Handle<Component>::Empty();
        }
        auto& world = GetWorld();
        auto& container = std::get<Settings::template GetComponentID<Component>()>(world.components);
        return container.GetHandle(components[Settings::template GetComponentID<Component>()]);
    }
    
    template<typename Component>
    void RemoveComponent() {
        assert(HasComponent<Component>());
        auto& world = GetWorld();
        
        world.removeActions.emplace_back([&world, this]() {
            auto& container = std::get<Settings::template GetComponentID<Component>()>(world.components);
            container.RemoveObject(components[Settings::template GetComponentID<Component>()]);
            auto activeComponentsBefore = activeComponents;
            activeComponents[Settings::template GetComponentID<Component>()] = false;
            meta::for_each_in_tuple_non_const(std::get< Settings::template GetComponentID<Component>() >(world.componentSystems), [&world, activeComponentsBefore, this] (auto& system) {
                auto& bitSet = world.systemBitsets[Settings::template GetSystemID<std::remove_const_t< std::remove_reference_t<decltype(system)> >>()];
                bool wasInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (wasInterest) {
                    system.ObjectRemoved(world.objects.GetHandle(index));
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
        
        world.createActions.emplace_back([&world, activeComponentsBefore, this]() {
            meta::for_each_in_tuple_non_const(std::get< Settings::template GetComponentID<Component>() >(world.componentSystems), [&world, activeComponentsBefore, this] (auto& system) {
                auto& bitSet = world.systemBitsets[Settings::template GetSystemID<std::remove_const_t< std::remove_reference_t<decltype(system)> >>()];
                bool isInterest = (activeComponentsBefore & bitSet) == bitSet;
                if (isInterest) {
                    system.ObjectAdded(world.objects.GetHandle(this->index));
                }
            });
        });
        
        return container.GetHandle(index);
    }
    
    void Remove() {
        if (isRemoved) return;
        isRemoved = true;
        auto& world = GetWorld();
        world.removeActions.emplace_back([&world, this]() {
            world.objects.RemoveObject(index);
        });
    }
};