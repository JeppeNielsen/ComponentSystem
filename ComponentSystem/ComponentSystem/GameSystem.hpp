//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include "IGameWorld.hpp"
#include "GameObject.hpp"
#include <vector>

template<typename... ComponentList>
class GameSystem {
public:
    
    using Components = meta::list<ComponentList...>;
    virtual ~GameSystem() {};
    
    using Systems = meta::list<>;

public:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;

public:
    const ObjectCollection& Objects() {
        return objects;
    }
};

class GameConcept : public GameSystem<> {

};