//
//  ScriptTest.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 06/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "libGameSystem.h"
#include <iostream>
#include <string>

struct Button {
    std::string name;
};

struct Sprite {
    int size;  
};

struct ButtonSystem : GameSystem<Transform, Button> {

    void ObjectAdded(GameObject* object) {
        std::cout << "ButtonSystem::ObjectAdded" << std::endl;
        object->GetComponent<Button>()->name = "Jeppe Nielsen";
    }

    void ObjectRemoved(GameObject* object) {
        std::cout << "ButtonSystem::ObjectRemoved" << std::endl;
    }

    void Update(float dt) {
        std::cout <<"ButtonSystem::Update"<< std::endl;
        for(auto o : Objects()) {
            std::cout <<"ButtonSystem::GetComponent<Transform>()->x = "<< o->GetComponent<Transform>()->x  << std::endl; 
            std::cout <<"ButtonSystem::GetComponent<Button>()->name = "<< o->GetComponent<Button>()->name  << std::endl; 
            //o->RemoveComponent<Button>();
           // o->RemoveComponent<Button>();
           // o->RemoveComponent<Button>();
        }
    }
};

struct TesterSystem : GameSystem<Transform> {
    
    void ObjectAdded(GameObject* object) {
         std::cout << "TesterSystem::ObjectAdded" << std::endl;  
    }  

    void ObjectRemoved(GameObject* object) {
        std::cout << "TesterSystem::ObjectRemoved" << std::endl;
    }
    
    void Update(float dt) {
        std::cout<<"TesterSystem::Update"<<std::endl;
    }
    
};

struct VelocitySystem : GameSystem<Transform, Velocity> {
    void ObjectAdded(GameObject* object) {
         std::cout << "VelocitySystem::ObjectAdded_script" << std::endl;  
    }  

    void ObjectRemoved(GameObject* object) {
        std::cout << "VelocitySystem::ObjectRemoved_script" << std::endl;
    }
    
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->x += o->GetComponent<Velocity>()->x * dt * 2;
        }
    }  
};


