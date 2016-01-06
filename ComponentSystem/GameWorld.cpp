//
//  GameWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 02/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include "GameWorld.hpp"

GameObject* GameWorld::CreateObject() {
    auto object = objects.CreateObject();
    object->object.instance = object;
    object->object.world = this;
    object->object.Reset();
    return &object->object;
}

