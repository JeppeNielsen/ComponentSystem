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
#include "Timer.hpp"

int main_nono() {

    GameWorld world;
    world.Initialize();
    
    Timer timer;
    
    timer.Begin();
    
    GameObject* sourceTransform = world.CreateObject();
    sourceTransform->AddComponent<Transform>()->x = 123;
    
    GameObject* object;
    for(int i=0; i<10; i++) {
        auto instance = world.CreateObject();
        instance->AddComponent<Transform>(sourceTransform);
        instance->AddComponent<Velocity>()->x = i+1;
        instance->AddComponent<Renderable>();
        if (i==0) object = instance;
    }
    
    world.Update(1);
    std::cout<<world.ObjectCount()<<std::endl;
    
    
    double time = timer.End();
    std::cout <<" Time = "<< time <<std::endl;
    
    timer.Begin();
    for (int i=0; i<100; ++i) {
        world.Update(1);
    }
    time = timer.End();
    std::cout <<" Time2 = "<< time <<std::endl;
    
    
    
    
    return 0;
}
