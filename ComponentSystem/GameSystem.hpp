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
#include "GameSettings.hpp"

template<typename Settings>
class GameObject;

template<typename... T>
class GameSystem {
public:
    using Components = meta::list<T...>;
protected:
    using Objects = std::vector<GameObject<DefaulSettings::Settings>>;
};