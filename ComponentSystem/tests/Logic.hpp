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

struct VelocitySystem : GameSystem<Transform, Velocity> {
    VelocitySystem();
    ~VelocitySystem();
    void Initialize(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
};

struct RenderSystem : GameSystem<Transform, Renderable> {
    RenderSystem();
    ~RenderSystem();
    void Initialize(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    
    Event<GameObject*> Added;
    Event<GameObject*> Removed;
};

struct TouchSystem : GameSystem<Transform, Touchable> {
    TouchSystem();
    ~TouchSystem();
    void Initialize(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    void Render();
};

struct GameWorldSettings : GameSettings<VelocitySystem, RenderSystem, TouchSystem> {};
#include "GameWorld.hpp"
