//
//  main.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSettings.hpp"
#include "GameSystem.hpp"
#include "GameWorld.hpp"
#include "metaLib.hpp"
#include <type_traits>
#include "ScriptWorld.hpp"
#include "Components.hpp"

template<typename T>
struct MoverSystem : GameSystem<T, Transform, Velocity> {

    using GameObject = GameObject<T>;

    void ObjectAdded(typename MoverSystem::GameObject* object) {
        std::cout << "Object added: " << object->template GetComponent<Transform>()->x << std::endl;
    }
    
    void ObjectRemoved(GameObject* object) {
        std::cout << "Object removed: " << object->template GetComponent<Transform>()->x << std::endl;
    }
    
    void Update(float dt) {
        for(auto o : this->Objects()) {
            o->template GetComponent<Transform>()->x += o->template GetComponent<Velocity>()->x * dt;
        }
    }
};

template<typename T>
struct RenderSystem : GameSystem<T, Transform, Renderable> {

    void ObjectAdded(GameObject<T>* object) {
        std::cout << "RenderSystem::ObjectAdded"<<std::endl;
    }

    void ObjectRemoved(GameObject<T>* object) {
        std::cout << "RenderSystem::ObjectRemoved"<<std::endl;
    }
    
    void Update(float dt) {
        int index = 0;
        for(auto o : this->Objects()) {
            std::cout << "Rendered object #"<<++index<<std::endl;
        }
    }
};

struct SpecifyGameSettings : GameSettings<
        MoverSystem<SpecifyGameSettings>,
        RenderSystem<SpecifyGameSettings>
        >
{ };

int main() {

    ScriptWorld scriptWorld;
    scriptWorld.SetComponentNames<SpecifyGameSettings>();
    scriptWorld.SetFiles(
    "ScriptExample.so",
    "/Projects/ComponentSystem/ComponentSystem/ScriptInclude",
    {
        "/Projects/ComponentSystem/ComponentSystem/ScriptCode/ScriptExample.hpp"
    },
    {
        "/Projects/ComponentSystem/ComponentSystem/ScriptInclude/Components.hpp"
    }
    );
    

    scriptWorld.Build();

    GameWorld<SpecifyGameSettings> world;
    
    scriptWorld.AddGameWorld(world);
    
    auto object = world.CreateObject();
    object->AddComponent<Transform>()->x = 4;
    object->AddComponent<Velocity>()->x = 10;
    object->AddComponent<Renderable>();
    object->AddScriptComponent(0);
    
    world.Update(0.01f);
    
    object->RemoveComponent<Renderable>();
    
    world.Update(0.01f);
    world.Update(0.01f);
    

    return 0;
}