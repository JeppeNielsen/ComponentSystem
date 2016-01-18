//
//  Logic.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 01/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include <iostream>
#include "GameWorld.hpp"

VelocitySystem::VelocitySystem() {
    //std::cout << "VelocitySystem ctor"<<std::endl;
}

VelocitySystem::~VelocitySystem() {
    //std::cout << "VelocitySystem dtor"<<std::endl;
}

void VelocitySystem::Initialize(GameWorld &world) {
    //std::cout << "VelocitySystem::Initialize"<<std::endl;
}

void VelocitySystem::ObjectAdded(GameObject *object) {
    //std::cout << "VelocitySystem::ObjectAdded"<<std::endl;
}

void VelocitySystem::ObjectRemoved(GameObject *object) {
    //std::cout << "VelocitySystem::ObjectRemoved"<<std::endl;
}

void VelocitySystem::Update(float dt) {
    //std::cout << "VelocitySystem::Update"<<std::endl;
}

RenderSystem::RenderSystem() {
    //std::cout<< "RenderSystem ctor"<<std::endl;
}

RenderSystem::~RenderSystem() {
    //std::cout<< "RenderSystem dtor"<<std::endl;
}

void RenderSystem::Initialize(GameWorld &world) {
    //std::cout<< "RenderSystem::Initialize"<<std::endl;
}

void RenderSystem::ObjectAdded(GameObject *object) {
    //std::cout<< "RenderSystem::ObjectAdded"<<std::endl;
    Added(object);
}

void RenderSystem::ObjectRemoved(GameObject *object) {
    //std::cout<< "RenderSystem::ObjectRemoved"<<std::endl;
    Removed(object);
}

void RenderSystem::Update(float dt) {
    //std::cout<< "RenderSystem::Update"<<std::endl;
}

TouchSystem::TouchSystem() {
    //std::cout<< "TouchSystem ctor"<<std::endl;
}

TouchSystem::~TouchSystem() {
    //std::cout<< "TouchSystem dtor"<<std::endl;
}

void TouchSystem::Initialize(GameWorld& world) {
    //std::cout<< "TouchSystem::Initialize"<<std::endl;
}

void TouchSystem::ObjectAdded(GameObject* object) {
    //std::cout<< "TouchSystem::ObjectAdded"<<std::endl;
}

void TouchSystem::ObjectRemoved(GameObject* object) {
    //std::cout<< "TouchSystem::ObjectRemoved"<<std::endl;
}

void TouchSystem::Update(float dt) {
    //std::cout<< "TouchSystem::Update"<<std::endl;
}

void TouchSystem::Render() {
    //std::cout<< "TouchSystem::Render"<<std::endl;
}
