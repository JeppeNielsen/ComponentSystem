//
//  System.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Components.hpp"

struct MoverSystem : GameSystem<Transform, Velocity> {
    
    
    struct InnerSystem : GameSystem<Transform> {
        void ObjectAdded(GameObject* object) {
            std::cout << "InnerSystem added: " << object->GetComponent<Transform>()->x << std::endl;
        
        }
        
        void ObjectRemoved(GameObject* object) {
            std::cout << "InnerSystem removed: " << object-> GetComponent<Transform>()->x << std::endl;
        
        }
    };

    using Systems = meta::list<InnerSystem>;
    
    
    InnerSystem* innerSystem;
    
    void Initialize(IGameWorld* world) {
        innerSystem = &world->GetSystem<InnerSystem>();
    }
    
    void ObjectAdded(GameObject* object) {
        std::cout << "MoverSystem added: " << object->GetComponent<Transform>()->x << std::endl;
        //std::cout << "Mover System :: Object Added"<<std::endl;
    }
    
    void ObjectRemoved(GameObject* object) {
        std::cout << "MoverSystem removed: " << object-> GetComponent<Transform>()->x << std::endl;
        //std::cout << "Mover System :: Object Removed"<<std::endl;
    }
    
    void Update(float dt) {
        //for(auto o : this->Objects()) {
        //    o->template GetComponent<Transform>()->x += o->template GetComponent<Velocity>()->x * dt;
        //}
    }
};

struct RenderSystem : GameSystem<Transform, Renderable> {

    //void ObjectAdded(GameObject* object) {
    //    std::cout << "RenderSystem::ObjectAdded"<<std::endl;
    //}

    //void ObjectRemoved(GameObject* object) {
    //    std::cout << "RenderSystem::ObjectRemoved"<<std::endl;
    //}
    
    /*void Update(float dt) {
        int index = 0;
        for(auto o : this->Objects()) {
            std::cout << "Rendered object #"<<++index<<std::endl;
        }
    }
    */
};




struct Gui : GameConcept {

    using Systems = meta::list<RenderSystem, MoverSystem>;
    
    IGameWorld* world;
    
    void Initialize(IGameWorld* world) {
        this->world = world;
    }
    
    GameObject* CreateGadget(float position) {
        auto object = world->CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        return object;
    }
};

