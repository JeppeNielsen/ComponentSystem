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
#include "Timer.hpp"

int main() {

    GameWorld world;
    
    Timer timer;
    
    {
    
    
    }
    
    timer.Begin();
    
    Handle<GameObject> object;
    for(int i=0; i<1000; i++) {
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
    
    
    world.Update(0.1f);
    
    double time = timer.End();
    
    std::cout <<" Time = "<< time <<std::endl;
    
    timer.Begin();
    
    for (int i=0; i<30000; ++i) {
        world.Update(1);
    }
    time = timer.End();
    
    std::cout <<" Time2 = "<< time <<std::endl;
    
    
    
    
    return 0;
}
