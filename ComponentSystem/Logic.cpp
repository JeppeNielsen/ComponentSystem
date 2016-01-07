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

void VelocitySystem::ObjectAdded(GameObject *object) {

}

void VelocitySystem::ObjectRemoved(GameObject *object) {
    //object->Remove();
}

void VelocitySystem::Update(float dt) {
    int i=0;
    for(auto e : Objects()) {
        //e->Remove();
        //std::cout<<"Update from VelocitySystem : "<< i << " vel: x = " << e->GetComponent<Velocity>()->x <<std::endl;
        i++;
        i+=e->GetComponent<Velocity>()->x;
    }
    index = i;
}

void RenderSystem::ObjectAdded(GameObject *object) {

}

void RenderSystem::ObjectRemoved(GameObject *object) {
    
}

void RenderSystem::Update(float dt) {

}

void AccSystem::ObjectAdded(GameObject *object) {

}

void AccSystem::ObjectRemoved(GameObject *object) {

}