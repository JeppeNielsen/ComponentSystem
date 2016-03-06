//
//  main.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

//#define SCRIPTING_ENABLED

#include "GameWorld.hpp"
#include "ScriptWorld.hpp"
#include "Systems.hpp"

int main() {

    ScriptWorld scriptWorld;
    //scriptWorld.SetWorldType<SpecificGameSettings>();
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
    
    GameWorldInitializer<MoverSystem, RenderSystem, Gui> initializer;
    
    GameWorld world;
    world.Initialize(initializer);
    
    scriptWorld.AddGameWorld(world);
    
    GameObject* object = world.CreateObject();
    
    object->AddComponent<Transform>();
    object->AddComponent<Velocity>();

    world.Update(1.0f);

{

    int breek = 6;

}
    
    
    /*
    
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

    */
    
    
    return 0;
}