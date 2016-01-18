//
//  testSerialization.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 17/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include <fstream>

int main_serialization() {

    GameWorld world;
    /*
    auto object = world.CreateObject();
    object->AddComponent<Transform>()->x = 4;
    object->GetComponent<Transform>()->y = 12;
    object->AddComponent<Renderable>();
    object->AddComponent<Velocity>()->x = 34234;
    
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
        
        
    
    }
    
    return 0;
}