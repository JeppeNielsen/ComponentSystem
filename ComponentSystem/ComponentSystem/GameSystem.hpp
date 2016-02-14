//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include <vector>

template<typename T>
class GameObject;

template<typename T>
class GameWorld;

template<typename T, typename... ComponentList>
class GameSystem {
public:
    friend class GameObject<T>;
    friend class GameWorld<T>;

    using GameObject = GameObject<T>;
    
    using Components = meta::list<ComponentList...>;
    virtual ~GameSystem() {};
    
    using Systems = meta::list<>;

private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    
    GameWorld<T>* world;
    
public:
    const ObjectCollection& Objects() {
        return objects;
    }

    GameWorld<T>& World() {
        return *world;
    }
};