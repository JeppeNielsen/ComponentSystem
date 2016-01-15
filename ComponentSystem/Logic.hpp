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
	std::string Name() const { return "Transform"; }
	int x;
	int y;
    
    //Transform() { std::cout<<"Transform ctor"<<std::endl; }
    //~Transform() { std::cout<<"Transform dtor"<<std::endl; }
};

struct Velocity {
	std::string Name() const { return "Velocity"; }
	int x;
	int y;
    //Velocity() { std::cout<<"Velocity ctor"<<std::endl; }
    //~Velocity() { std::cout<<"Velocity dtor"<<std::endl; }
};

struct Renderable {
	std::string Name() const { return "Renderable"; }
	int imageNo;
    //Renderable() { std::cout<<"Renderable ctor"<<std::endl; }
    //~Renderable() { std::cout<<"Renderable dtor"<<std::endl; }
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
};

struct AccSystem : GameSystem<Velocity, Renderable> {
    AccSystem();
    ~AccSystem();
    void Initialize(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
};

struct GameWorldSettings : GameSettings<VelocitySystem, RenderSystem, AccSystem> {};
#include "GameWorld.hpp"
