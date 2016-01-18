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

struct Transform {
	int x;
	int y;
    
    //Transform() { std::cout<<"Transform ctor"<<std::endl; }
    //~Transform() { std::cout<<"Transform dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN()
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Velocity {
	int x;
	int y;
    //Velocity() { std::cout<<"Velocity ctor"<<std::endl; }
    //~Velocity() { std::cout<<"Velocity dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN()
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Renderable {
	int imageNo;
    //Renderable() { std::cout<<"Renderable ctor"<<std::endl; }
    //~Renderable() { std::cout<<"Renderable dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN()
    TYPE_FIELD(imageNo)
    TYPE_FIELDS_END
};

struct Touchable {
	bool clickThrough;
    //Touchable() { std::cout<<"Renderable ctor"<<std::endl; }
    //~Touchable() { std::cout<<"Renderable dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN()
    TYPE_FIELD(clickThrough)
    TYPE_FIELDS_END
};

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
