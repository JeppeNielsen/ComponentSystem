//
//  testHierarchy.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 18/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"

int main_testHierarchy() {

    GameWorld<Settings> world;
    
    auto parent = world.CreateObject();
    
    auto child = world.CreateObject();
    child->Parent = parent;
    
    std::cout << "parent->Children().size = "<< parent->Children().size()<<std::endl;
    


    return 0;
}