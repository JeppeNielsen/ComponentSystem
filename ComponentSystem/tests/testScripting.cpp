//
//  testScripting.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 25/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "Logic.hpp"
#include "ScriptWorld.hpp"
#include "Timer.hpp"

int main() {

    ScriptWorld scriptWorld;
    scriptWorld.SetFiles(
    "ScriptExample.so",
    "/Projects/ComponentSystem/ComponentSystem/ScriptInclude",
    {
        "/Projects/ComponentSystem/ComponentSystem/ScriptCode/ScriptExample.hpp"
    },
    {
        "/Projects/ComponentSystem/ComponentSystem/tests/Components.hpp"
    }
    );
    
    Timer timer;
    timer.Begin();
    scriptWorld.Build();
    double buildTime = timer.End();
    std::cout << "Build complete, time = " << buildTime <<"s"<< std::endl;
    
    GameWorld world;
    scriptWorld.AddGameWorld(world);
    
    GameObject* go = world.CreateObject();
    go->AddComponent<Transform>()->x = 100;
    go->AddScriptComponent(0);
    
    world.Update(0.01f);
    
    scriptWorld.RemoveGameWorld(world);
    
    return 0;
}
