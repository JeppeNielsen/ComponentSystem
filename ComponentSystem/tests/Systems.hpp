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

template<typename T>
struct MoverSystem : GameSystem<T, Transform, Velocity> {


    struct MoverInnerSystem : GameSystem<T, Transform> {
    
        
        struct OtherSystem : GameSystem<T, Velocity> {
        
            void ObjectAdded(GameObject<T>* object) {
                
            }
            
            void ObjectRemoved(GameObject<T>* object) {
            
            }
            
        };
        
        using Systems = meta::list<OtherSystem>;
        
        void ObjectAdded(GameObject<T>* object) {
          //  auto& t = this->World().template GetSystem<OtherSystem>();
            
          //  int bla = 4;
          //  bla++;
            
        }
        
        void ObjectRemoved(GameObject<T>* object) {
        
        }
        
    };
    
    using Systems = meta::list<MoverInnerSystem>;

    //TransformSystem* transformSystem;
    
    /*void Initialize() {
        transformSystem = &this->World().template GetSystem<TransformSystem>();
        int hej = 3;
        hej++;
        std::cout << "MoverSystem"<<std::endl;
    }
    */
    
    void ObjectAdded(GameObject<T>* object) {
    
    
        std::cout << "Object added: " << object->template GetComponent<Transform>()->x << std::endl;
    }
    
    void ObjectRemoved(GameObject<T>* object) {
        std::cout << "Object removed: " << object->template GetComponent<Transform>()->x << std::endl;
        
    }
    
    void Update(float dt) {
        for(auto o : this->Objects()) {
            o->template GetComponent<Transform>()->x += o->template GetComponent<Velocity>()->x * dt;
        }
        
        //std::cout <<" TransformSystem::Count = " << transformSystem->template Objects().size()<< std::endl;
    }
};

template<typename T>
struct RenderSystem : GameSystem<T, Transform, Renderable> {

    void ObjectAdded(GameObject<T>* object) {
        std::cout << "RenderSystem::ObjectAdded"<<std::endl;
    }

    void ObjectRemoved(GameObject<T>* object) {
        std::cout << "RenderSystem::ObjectRemoved"<<std::endl;
    }
    
    void Update(float dt) {
        int index = 0;
        for(auto o : this->Objects()) {
            std::cout << "Rendered object #"<<++index<<std::endl;
        }
    }
};


template<typename T>
struct Gui : GameConcept<T> {

    using Systems = meta::list<RenderSystem<T>, MoverSystem<T>>;

    //void ObjectAdded(GameObject<T>* object) {}
    //void ObjectRemoved(GameObject<T>* object) {}
    
    GameObject<T>* CreateGadget(float position) {
        auto object = this->World().CreateObject();
        return object;
    }
    
};

