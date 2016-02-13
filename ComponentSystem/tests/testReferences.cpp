//
//  testReferences.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 11/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "Logic.hpp"

int main_nonol() {

    World world;
    world.Initialize();
    
    Object* test = 0;
    {
        World content;
        Object* expensiveTransform = content.CreateObject();
        expensiveTransform->AddComponent<Transform>()->x = 123456;
        content.Update(0);
        
        test = world.CreateObject();
        test->AddComponent<Velocity>();
        test->AddComponent<Transform>(expensiveTransform);
        test->AddComponent<Renderable>();
        
        world.Update(0);
        world.Render();
    }
    
    //std::cout<<test->GetComponent<Transform>()->x<<std::endl;
    return 0;
}
