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
#include <signal.h>
#include <setjmp.h>

jmp_buf errorLabel;

void ErrorInScriptsDetected (int signum)
{
   longjmp(errorLabel, 1);
}

int main() {

    signal(SIGSEGV, ErrorInScriptsDetected);
    

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
    
    {
        GameWorld world;
        scriptWorld.AddGameWorld(world);
        
        GameObject* go = world.CreateObject();
        go->AddComponent<Transform>()->x = 100;
        go->AddScriptComponent(0);
        world.Update(0);
        
        switch (setjmp (errorLabel)) {
            case 0: {
                
                for(int i=0;i<100; ++i) {
                    world.Update(0.01f);
                }

                
                break;
            }
            case 1: {
                
                std::cout << "Scripts crashed!"<< std::endl;
            }
        }
        
        scriptWorld.RemoveGameWorld(world);
    }
    
    return 0;
}
