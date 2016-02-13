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

template<typename T, typename... Systems>
class GameSystem {
public:
    friend class GameObject<T>;

    using GameObject = GameObject<T>;
    
    using Components = meta::list<Systems...>;
    virtual ~GameSystem() {};
private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    
    
protected:
    const ObjectCollection& Objects() {
        return objects;
    }
};