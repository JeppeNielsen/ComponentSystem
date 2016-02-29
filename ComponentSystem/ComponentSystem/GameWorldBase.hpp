//
//  GameWorldBase.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 28/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>

class GameWorldBase {
public:
    std::vector<std::array<std::function<void*(void*)>,2>> commands;
};