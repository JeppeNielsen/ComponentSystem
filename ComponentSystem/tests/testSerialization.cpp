//
//  testSerialization.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "GameSystem.hpp"
#include "GameWorld.hpp"
#include "ScriptWorld.hpp"
#include "Systems.hpp"

using Settings = GameSettings<MoverSystem, RenderSystem, Gui>;





template<typename T>
struct Debug;

int main() {

  //  Debug<SpecificGameSettings::Systems> hej;

    //std::cout << GameComponent::GetComponentID<OtherComponent>()<<std::endl;
    
    GameWorld<Settings> world;
    world.Initialize();
    
    auto gadget = world.GetSystem<Gui>().CreateGadget(0);
    gadget->AddComponent<Velocity>();
    
    gadget->ToJson(std::cout);
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