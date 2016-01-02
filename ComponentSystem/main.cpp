//
//  tests.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "metaLib.hpp"
#include "Logic.hpp"
#include "GameWorld.hpp"

int main() {

    GameWorld world;
    
    {
    
    
    }
    
    Handle<GameObject> object;
    for(int i=0; i<10; i++) {
        auto instance = world.CreateObject();
        instance->AddComponent<Transform>();
        instance->AddComponent<Velocity>()->x = i+1;
        instance->AddComponent<Renderable>();
        if (i==0) object = instance;
    }
    
    //object->RemoveComponent<Transform>();
    bool hasTransform1 = object->HasComponent<Transform>();
    
    world.Update(1);
    
    //object->RemoveComponent<Transform>();
    world.Update(1);
    
    auto& renderSystem = world.GetSystem<RenderSystem>();
    
    int vel = object->GetComponent<Velocity>()->x;
    
    //object->RemoveComponent<Velocity>();
    //object->RemoveComponent<Velocity>();
    
    object->Remove();
    
    auto trans = object->GetComponent<Transform>();
    if (trans) {
        trans->x = 3;
        trans->y = 100;
    }
    
    world.Update(0.1f);

    return 0;
}
