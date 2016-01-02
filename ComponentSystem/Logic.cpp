//
//  Logic.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 01/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include <iostream>
#include "GameWorld.hpp"

void VelocitySystem::Update(float dt) {
    int i=0;
    for(auto e : Objects()) {
        std::cout<<"Update from VelocitySystem : "<< i << " vel: x = " << e->GetComponent<Velocity>()->x <<std::endl;
        i++;
    }
}
