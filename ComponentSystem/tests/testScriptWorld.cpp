//
//  main.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#define SCRIPTING_ENABLED

#include "GameSettings.hpp"
#include "GameSystem.hpp"
#include "GameWorld.hpp"
#include "metaLib.hpp"
#include <type_traits>
#include "ScriptWorld.hpp"
#include "Components.hpp"

template<typename T>
struct MoverSystem : GameSystem<T, Transform, Velocity> {

    struct TransformSystem : GameSystem<T, Transform> {
    
        
        struct OtherSystem : GameSystem<T, Velocity> {
        
            void ObjectAdded(GameObject<T>* object) {
                
            }
            
            void ObjectRemoved(GameObject<T>* object) {
            
            }
            
        };
        
        using Systems = meta::list<OtherSystem>;
        
        void ObjectAdded(GameObject<T>* object) {
            auto& t = this->World().template GetSystem<OtherSystem>();
            
            int bla = 4;
            bla++;
            
        }
        
        void ObjectRemoved(GameObject<T>* object) {
        
        }
        
    };
    
    using Systems = meta::list<TransformSystem, typename TransformSystem::OtherSystem>;

    TransformSystem* transformSystem;
    
    void Initialize() {
        transformSystem = &this->World().template GetSystem<TransformSystem>();
        
        
        
        int hej = 3;
        hej++;
        
        std::cout << "MoverSystem"<<std::endl;
        
    }

    void ObjectAdded(GameObject<T>* object) {
    
    
        std::cout << "Object added: " << object->template GetComponent<Transform>()->x << std::endl;
    }
    
    void ObjectRemoved(GameObject<T>* object) {
        std::cout << "Object removed: " << object->template GetComponent<Transform>()->x << std::endl;
        
    }
    
    void Update(float dt) {
        for(auto o : this->Objects()) {
            o->template GetComponent<Transform>()->x += o->template GetComponent<Velocity>()->x * dt;
        }
        
        std::cout <<" TransformSystem::Count = " << transformSystem->template Objects().size()<< std::endl;
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

struct SpecificGameSettings : GameSettings<
        MoverSystem<SpecificGameSettings>,
        RenderSystem<SpecificGameSettings>
        >
{ };

int main_TestScriptWorld() {

    ScriptWorld scriptWorld;
    scriptWorld.SetWorldType<SpecificGameSettings>();
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

    GameWorld<SpecificGameSettings> world;
    world.Initialize();
    
    scriptWorld.AddGameWorld(world);
    
    
    
    {
        auto obj = world.CreateObject();
        obj->AddComponent<Transform>();
        
        obj->GetComponent<Transform>()->x = 1234;
        
        
        std::cout<< obj->GetComponent<Transform>()->x << std::endl;
        
        obj->RemoveComponent<Transform>();
        
        world.Update(0);
    
        obj->AddComponent<Transform>();
        
        std::cout<< obj->GetComponent<Transform>()->x << std::endl;
    
        obj->Remove();
        
        world.Update(0);
    
    }
    
    auto object = world.CreateObject();
    object->AddComponent<Transform>()->x = 4;
    object->AddComponent<Velocity>()->x = 10;
    object->AddComponent<Renderable>();
    object->AddScriptComponent(0);
    object->AddScriptComponent(1);
    
    
    
    auto obj2 = world.CreateObject();
    obj2->AddComponent<Transform>();
    
    
    world.Update(0.01f);
    
    object->RemoveComponent<Renderable>();
    
    world.Update(0.01f);
    world.Update(0.01f);
    
    {
    TypeInfo typeInfo = scriptWorld.GetTypeInfo(*object, 0);
    
    minijson::writer_configuration config;
    config.pretty_printing(true);
    minijson::object_writer w(std::cout, config);
    
    typeInfo.Serialize(w);
    w.close();
    
    object->RemoveScriptComponent(0);
    
    world.Update(1.0f);
    }
    
    
    object->AddScriptComponent(0);
    
    {
    TypeInfo typeInfo = scriptWorld.GetTypeInfo(*object, 0);
    
    minijson::writer_configuration config;
    config.pretty_printing(true);
    minijson::object_writer w(std::cout, config);
    
    typeInfo.Serialize(w);
    w.close();
    
    }

    
    
    
    return 0;
}