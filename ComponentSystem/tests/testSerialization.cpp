//
//  testSerialization.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "GameWorld.hpp"
#include "ScriptWorld.hpp"
#include "Systems.hpp"

struct SpecificGameSettings : GameSettings<
        MoverSystem<SpecificGameSettings>,
        RenderSystem<SpecificGameSettings>,
        Gui<SpecificGameSettings>
        >
{ };

//template<typename T>
//struct Debug;

using World = GameWorld<SpecificGameSettings>;
using Object = GameObject<SpecificGameSettings>;

int main() {

  //  Debug<SpecificGameSettings::Systems> hej;

    World world;
    
    world.Initialize();
    
    auto& gui = world.GetSystem<Gui<SpecificGameSettings>>();
    
    auto bla = gui.CreateGadget(10);
    
   
    auto object = world.CreateObject();
    
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
    
    
    return 0;
}