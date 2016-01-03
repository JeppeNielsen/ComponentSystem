//
//  GameWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 02/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include "GameWorld.hpp"

typename GameWorld::Worlds GameWorld::worlds;

GameObject* GameWorld::CreateObject() {
    int index;
    auto object = objects.CreateObject();
    object->object.instance = object;
    object->object.world = this;
    return &object->object;
}

