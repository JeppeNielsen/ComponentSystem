//
//  testSerialization.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include <fstream>

int main() {

    GameWorld world;
    /*
    auto object = world.CreateObject();
    object->AddComponent<Transform>()->x = 4;
    object->GetComponent<Transform>()->y = 12;
    object->AddComponent<Renderable>();
    object->AddComponent<Velocity>()->x = 34234;
    
    object->GetComponent<Velocity>()->info.HejMedDig = 5748;
    
    {
        object->ToJson(std::cout, [] (GameObject* object, int componentType) {
            return true;//componentType == GameWorldSettings::GetComponentID<Transform>();
        });
    }
    
    if (false)
    {
        std::ofstream file;
        file.open ("TestObject.json");
        object->ToJson(file, [] (GameObject* object, int componentType) {
            return true;//componentType == GameWorldSettings::GetComponentID<Transform>();
        });
        file.close();
    }
    */
    
    //if (false)
    {
        std::ifstream file;
        file.open ("TestObject.json");
        GameObject* loadedObject = world.CreateObject(file, [] (GameObject* o) {
        
        });
        file.close();
        
        loadedObject->ToJson(std::cout, [] (GameObject* object, int componentType) {
            return true;//componentType == GameWorldSettings::GetComponentID<Transform>();
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