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
    
    Transform() {}
    Transform(int x, int y) : x(x), y(y) {
    
    }
};

struct Velocity {
	std::string Name() const { return "Velocity"; }
	int x;
	int y;
};

struct Renderable {
	std::string Name() const { return "Renderable"; }
	int imageNo;
};

struct VelocitySystem : public GameSystem<Transform, Velocity> {
    void Update(float dt);
    
    void ObjectAdded(Handle<GameObject> object) {
        //std::cout<<"VelocitySystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject> object) {
        //std::cout<<"VelocitySystem ObjectRemoved"<<std::endl;
    }
};

struct RenderSystem : public GameSystem<Transform, Renderable> {
    int index;
    void Update(float dt) {
        //std::cout<<"Update from RenderSystem"<<std::endl; index++;
    }
    
    void ObjectAdded(Handle<GameObject> object) {
        //std::cout<<"RenderSystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject> object) {
        //std::cout<<"RenderSystem ObjectRemoved"<<std::endl;
    }
};

struct AccSystem : public GameSystem<Velocity, Renderable> {
    //void Update(float dt) { std::cout<<"Update from AccSystem"<<dt<<std::endl; }
    
    void ObjectAdded(Handle<GameObject> object) {
        //std::cout<<"AccSystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject> object) {
        //std::cout<<"AccSystem ObjectRemoved"<<std::endl;
    }
};

struct GameWorldSettings : GameSettings<RenderSystem, VelocitySystem, AccSystem> {};
