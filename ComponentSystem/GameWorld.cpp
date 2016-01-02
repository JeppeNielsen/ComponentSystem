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

Handle<GameObject> GameWorld::CreateObject() {
    int index = objects.CreateObject();
    objects.GetObject(index)->index = index;
    return objects.GetHandle(index);
}

