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
#include <iostream>
#include "GameObjectBase.hpp"
#include "IDHelper.hpp"

class GameWorldBase {
protected:
    virtual ~GameWorldBase() { }
    
    std::vector<std::array<std::function<void*(void*)>,2>> commands;
    std::vector<std::function<void*()>> getSystemCommands;
public:
    virtual GameObjectBase* CreateObject() = 0;
    virtual GameObjectBase* CreateObject(std::istream &jsonStream, std::function<void(GameObjectBase*)> onCreated) = 0;

    template<typename System>
    System& GetSystem() {
        return *((System*)getSystemCommands[IDHelper::GetSystemID<System>()]());
    }
};