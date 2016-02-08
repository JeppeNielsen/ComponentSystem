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

struct ButtonSystem : public GameSystem<Button> {

    void ObjectAdded(GameObject* object) {

        object->AddComponent<Transform>()->x = 455;
        object->AddComponent<Velocity>();
        
        std::cout <<"Object added"<< std::endl;
        
        
           
        object->AddComponent<Button>()->name = "Jeppe Nielsen";
        
        
        
        std::cout <<"object->GetComponent<Button>()->name = "<< object->GetComponent<Button>()->name << std::endl;
    }
    

    void Update(float dt) {
        std::cout <<"Main"<< std::endl;
        std::cout <<"2nd line"<< std::endl;
    }

};
