
#pragma once
#include "libGameSystem.h"
#include <iostream>
#include <string>

struct ScriptTransform {
    int x;
    int y;
};

struct ScriptVelocity {
    int vx;
    int vy;
};

struct ScriptVelocitySystem : GameSystem<ScriptTransform, ScriptVelocity> {
    
    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Transform>()->x++;
    }
    
    void Update(float dt) override {
        std::cout << "ScriptVelocitySystem::Update " << dt << std::endl;
    }
    
};

struct ScriptRenderSystem : GameSystem<ScriptTransform> {

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Transform>()->x = 67890+54;
    }

    void Update(float dt) override {
        std::cout << "ScriptRenderSystem::Update " << dt << std::endl;
    }
};


struct ScriptMoverSystem : GameSystem<ScriptVelocity> {

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Transform>()->x = 55555;
    }

    void Update(float dt) override {
        std::cout << "ScriptMoverSystem::Update " << dt << std::endl;
    }
};
