//
//  testSerialization.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "GameWorld.hpp"
#include "Systems.hpp"

//#include "ScriptWorld.hpp"

template<typename T>
struct Debug;



int main() {

    GameWorldInitializer<MoverSystem, RenderSystem, Gui> initializer;

    GameWorld world;
    world.Initialize(initializer);
    
    GameObject* gadget = world.GetSystem<Gui>().CreateGadget(10);
    
    gadget->ToJson(std::cout);
    
    GameObject* object = world.CreateObject();
    
    object->AddComponent<Transform>()->x = 123;
    object->AddComponent<Velocity>()->info.TestVar =343;
    object->AddComponent<Renderable>()->imageNo = 43;
    
    
    
    {
        std::ofstream file;
        file.open ("TestObject.json");
        object->ToJson(file);
        file.close();
    }
    
    
    
    {
        std::ifstream file;
        file.open("TestObject.json");
        GameObject* loadedObject = world.CreateObject(file, [](GameObject* go) { });
        file.close();
        
        loadedObject->ToJson(std::cout);
    }
    
    
    
    
    
    
    
    
    
    //std::cout << object->GetComponent<Transform>()->x<<std::endl;
    
    
    
    
    
    
    
    
    world.Update(1.0f);
    
    {
        int breek = 3;
    
    }
    
    
    
    
    
    
    
    

  //  Debug<SpecificGameSettings::Systems> hej;

    //std::cout << GameComponent::GetComponentID<OtherComponent>()<<std::endl;
    /*
    GameWorld<Settings> world;
    world.Initialize();
    
    auto gadget = world.GetSystem<Gui>().CreateGadget(0);
    gadget->AddComponent<Velocity>();
    
    auto child = world.GetSystem<Gui>().CreateGadget(0);
    
    child->Parent = gadget;
    
    gadget->ToJson(std::cout);
    */
    
    //, [] (auto o, auto t) {
    //    return t != GameComponent::GetComponentID<Transform>();
    //});
    
    //        return true;//componentType == WorldSettings::GetComponentID<Transform>();
    //});
    
    
    
    /*
    object->AddComponent<Transform>()->x = 4;
    object->GetComponent<Transform>()->y = 12;
    object->AddComponent<Renderable>();
    object->AddComponent<Velocity>()->x = 34234;
    object->GetComponent<Velocity>()->info.TestVar = 5748;
    
    {
        auto child = world.CreateObject();
        child->AddComponent<Transform>()->x = 1337;
        child->Parent = object;
    }
    
    if (false)
    {
        std::ofstream file;
        file.open ("TestObject.json");
        object->ToJson(file, [] (Object* object, int componentType) {
            return true;//componentType == WorldSettings::GetComponentID<Transform>();
        });
        file.close();
    }
    
    
    if (false)
    {
        object->ToJson(std::cout, [] (Object* object, int componentType) {
            return true;//componentType == WorldSettings::GetComponentID<Transform>();
        });
    }
    
    {
        std::cout << " loaded from stream "<<std::endl;
    
        
    }
    
    
    
    
    
    
    {
        std::ifstream file;
        file.open ("TestObject.json");
        Object* loadedObject = world.CreateObject(file, [] (Object* o) {
        
        });
        file.close();
        
        loadedObject->ToJson(std::cout, [] (Object* object, int componentType) {
            return true;//componentType == WorldSettings::GetComponentID<Transform>();
        });
        
        std::cout << "Has Transform : " << loadedObject->HasComponent<Transform>() << std::endl;
        std::cout << "Has Velocity : " << loadedObject->HasComponent<Velocity>() << std::endl;
        std::cout << "Has Renderable : " << loadedObject->HasComponent<Renderable>() << std::endl;
        
        int testVar = loadedObject->GetComponent<Velocity>()->info.TestVar;
        
        std::cout << testVar<< std::endl;
        
        world.Update(0);
    }
    */
    
    return 0;
}