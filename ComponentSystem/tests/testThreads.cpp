//
//  testThreads.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 07/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <thread>
#include "Timer.hpp"

struct GameObject {
    int bla;
};

#include <signal.h>     /* signal, raise, sig_atomic_t */


std::thread* thread;

void SignalHandler (int signum)
{
    std::cout << "EXIT"<<std::endl;
    pthread_kill(thread->native_handle(), 0);
//    throw  1;


signal(signum, SIG_DFL);
//kill(getpid(), signum);

}

int main_threads() {

    static sigset_t   signal_mask;

    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGINT);
    sigaddset (&signal_mask, SIGTERM);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);

 
    int counter = 0;
    std::thread t;
    
    thread = &t;

     signal(SIGSEGV, SignalHandler);
 
    
    t = std::thread( [] {
    
        try {
    
            GameObject* obj = 0;
            obj->bla = 4;
        
        
        }
        catch (int hej) {
            
        }
    });


    
    std::thread t2( [&counter] {
        while (counter<1000000) {
            counter++;
        }
    });
    
    t.join();
    t2.join();
    
    std::cout << "counter = " << counter<<std::endl;

    return 0;
}
