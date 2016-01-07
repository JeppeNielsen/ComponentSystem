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

struct VelocitySystem : GameSystem<Transform, Velocity> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    int index;
};

struct RenderSystem : GameSystem<Transform, Renderable> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    int index;
};

struct AccSystem : GameSystem<Velocity, Renderable> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
};

struct GameWorldSettings : GameSettings<RenderSystem, VelocitySystem, AccSystem> {};
