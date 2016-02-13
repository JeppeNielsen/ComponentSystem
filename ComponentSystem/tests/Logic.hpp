//
//  Logic.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 01/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <iostream>
#include "GameSettings.hpp"
#include "Event.hpp"
#include "Components.hpp"
#include "GameWorld.hpp"


template<typename Settings>
struct VelocitySystem : GameSystem<Settings, Transform, Velocity> {
    VelocitySystem() {
    
    }
    ~VelocitySystem() {
    
    }
    void Initialize() {
    
    }
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Transform>();
    }
    void ObjectRemoved(GameObject* object) {
    
    }
    void Update(float dt) {
    
    }
};

template<typename Settings>
struct RenderSystem : GameSystem<Settings, Transform, Renderable> {
    RenderSystem() {
    
    }
    ~RenderSystem() {
    
    }
    void Initialize() {
    
    }
    void ObjectAdded(GameObject* object) {
    
    }
    void ObjectRemoved(GameObject* object) {
    
    }
    void Update(float dt) {
    
    }
    
    Event<GameObject*> Added;
    Event<GameObject*> Removed;
};

template<typename Settings>
struct TouchSystem : GameSystem<Settings, Transform, Touchable> {
    TouchSystem() {
    
    }
    ~TouchSystem() {
    
    }
    void Initialize() {
    
    }
    void ObjectAdded(GameObject* object) {
    
    }
    void ObjectRemoved(GameObject* object) {
    
    }
    void Update(float dt) {
    
    }
};


using World = GameWorld<Settings>;

using Object = GameObject<Settings>;
