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
    std::cout << "VelocitySystem ctor"<<std::endl;
}

VelocitySystem::~VelocitySystem() {
    std::cout << "VelocitySystem dtor"<<std::endl;
}

void VelocitySystem::Initialize(GameWorld &world) {
    std::cout << "VelocitySystem::Initialize"<<std::endl;
}

void VelocitySystem::ObjectAdded(GameObject *object) {
    std::cout << "VelocitySystem::ObjectAdded"<<std::endl;
}

void VelocitySystem::ObjectRemoved(GameObject *object) {
    std::cout << "VelocitySystem::ObjectRemoved"<<std::endl;
}

void VelocitySystem::Update(float dt) {
    std::cout << "VelocitySystem::Update"<<std::endl;
}

RenderSystem::RenderSystem() {
    std::cout<< "RenderSystem ctor"<<std::endl;
}

RenderSystem::~RenderSystem() {
    std::cout<< "RenderSystem dtor"<<std::endl;
}

void RenderSystem::Initialize(GameWorld &world) {
    std::cout<< "RenderSystem::Initialize"<<std::endl;
}

void RenderSystem::ObjectAdded(GameObject *object) {
    std::cout<< "RenderSystem::ObjectAdded"<<std::endl;
}

void RenderSystem::ObjectRemoved(GameObject *object) {
    std::cout<< "RenderSystem::ObjectRemoved"<<std::endl;
}

void RenderSystem::Update(float dt) {
    std::cout<< "RenderSystem::Update"<<std::endl;
}

AccSystem::AccSystem() {
    std::cout<< "AccSystem ctor"<<std::endl;
}

AccSystem::~AccSystem() {
    std::cout<< "AccSystem dtor"<<std::endl;
}

void AccSystem::Initialize(GameWorld& world) {
    std::cout<< "AccSystem::Initialize"<<std::endl;
}

void AccSystem::ObjectAdded(GameObject* object) {
    std::cout<< "AccSystem::ObjectAdded"<<std::endl;
}

void AccSystem::ObjectRemoved(GameObject* object) {
    std::cout<< "AccSystem::ObjectRemoved"<<std::endl;
}

void AccSystem::Update(float dt) {
    std::cout<< "AccSystem::Update"<<std::endl;
}

void AccSystem::Render() {
    std::cout<< "AccSystem::Render"<<std::endl;
}
