//
//  testReferences.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 11/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "Logic.hpp"
#include "GameWorld.hpp"

int main() {

    GameWorld world;
    GameObject* test = 0;
    {
    GameWorld content;
    GameObject* expensiveTransform = content.CreateObject();
    expensiveTransform->AddComponent<Transform>()->x = 123456;
    content.Update(0);
    
    world.Initialize();
    test = world.CreateObject();
    test->AddComponent<Transform>(expensiveTransform);
    
    }
    
    
    std::cout<<test->GetComponent<Transform>()->x<<std::endl;
    

    return 0;
}
