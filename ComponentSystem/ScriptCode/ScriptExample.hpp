//
//  ScriptExample.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include <iostream>

struct Sprite {
    float width;
    float height;
};

struct SpriteSystem : GameSystem<Transform, Sprite> {
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Sprite>()->width = 0;
        std::cout << "SpriteSystem::ObjectAdded"<<std::endl;
    }

    void ObjectRemoved(GameObject* object) {
        std::cout << "SpriteSystem::ObjectRemoved"<<std::endl;
    }
    
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Sprite>()->width += o->GetComponent<Transform>()->x;
            std::cout << "o->GetComponent<Sprite>()->width = "<<o->GetComponent<Sprite>()->width<<std::endl;
        }
        std::cout << "SpriteSystem::Update"<<std::endl;
    }
};

struct MoverSystem : GameSystem<Transform> {
    
    void ObjectAdded(GameObject* object) {
        std::cout << "MoverSystem::ObjectAdded"<<std::endl;
    }
        
    void ObjectRemoved(GameObject* object) {
        std::cout << "MoverSystem::ObjectRemoved"<<std::endl;
    }

    

};