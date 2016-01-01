//
//  tests.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "GameWorld.hpp"
#include "Container.hpp"

struct RenderSystem;
struct VelocitySystem;
struct AccSystem;

using Systems = meta::list<RenderSystem, VelocitySystem, AccSystem>;
using Settings = GameSettings<Systems>;

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
    void Update(float dt) { std::cout<<"Update from VelocitySystem"<<std::endl; }
    
    void ObjectAdded(Handle<GameObject<Settings>> object) {
        std::cout<<"VelocitySystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject<Settings>> object) {
        std::cout<<"VelocitySystem ObjectRemoved"<<std::endl;
    }
};

struct RenderSystem : public GameSystem<Transform, Renderable> {
    int index;
    void Update(float dt) {
        std::cout<<"Update from RenderSystem"<<std::endl; index++;
    }
    
    void ObjectAdded(Handle<GameObject<Settings>> object) {
        std::cout<<"RenderSystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject<Settings>> object) {
        std::cout<<"RenderSystem ObjectRemoved"<<std::endl;
    }
};

struct AccSystem : public GameSystem<Velocity, Renderable> {
    void Update(float dt) { std::cout<<"Update from AccSystem"<<dt<<std::endl; }
    
    void ObjectAdded(Handle<GameObject<Settings>> object) {
        std::cout<<"AccSystem ObjectAdded"<<std::endl;
    }
    
    void ObjectRemoved(Handle<GameObject<Settings>> object) {
        std::cout<<"AccSystem ObjectRemoved"<<std::endl;
    }
};

struct Draggable {
    bool canDrag;
    int dragPosition;
};

int main() {
    
    GameWorld<Settings> world;

    using ComponentSystems = meta::as_list<decltype(meta::FindComponentSystems<Systems, Settings::UniqueComponents>{}.Iterate())>;
    using ComponentSystemsTuple = meta::mp_rename<ComponentSystems, std::tuple>;
    
    ComponentSystemsTuple bla {};

    //std::get<0>(bla).
    
    auto object = world.CreateObject();
    object->AddComponent<Transform>();
    object->AddComponent<Velocity>();
    object->AddComponent<Renderable>();
    
    auto copy = object;
    
    //object->RemoveComponent<Transform>();
    bool hasTransform1 = object->HasComponent<Transform>();
    
    world.Update(1);
    
    //object->RemoveComponent<Transform>();
    world.Update(1);
    
    auto& renderSystem = world.GetSystem<RenderSystem>();
    
    object->RemoveComponent<Velocity>();
    
    auto trans = object->GetComponent<Transform>();
    if (trans) {
        trans->x = 3;
        trans->y = 100;
    }
    
    world.Update(0.1f);

    return 0;
}
