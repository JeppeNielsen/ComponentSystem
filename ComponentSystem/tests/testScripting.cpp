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

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %ld, si_addr = %p si_signo = %i, si_code = %i\n",
			(long)siginfo->si_pid, (long)siginfo->si_uid, siginfo->si_addr, siginfo->si_signo, siginfo->si_code);
    
   longjmp(errorLabel, 1);
}

int main() {

    struct sigaction act;
 
	memset (&act, '\0', sizeof(act));
 
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &hdl;
 
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
 
	if (sigaction(SIGSEGV, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}
    
    if (sigaction(SIGABRT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}


    //signal(SIGSEGV, ErrorInScriptsDetected);
    

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
//        world.Update(0);
        
        switch (setjmp (errorLabel)) {
            case 0: {
                
                for(int i=0;i<20; ++i) {
                    world.Update(0.01f);
                }
                
                sleep(10);
                scriptWorld.RemoveGameWorld(world);
                scriptWorld.Build();
                scriptWorld.AddGameWorld(world);
                go->AddScriptComponent(0);
                
                for(int i=0;i<20; ++i) {
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
