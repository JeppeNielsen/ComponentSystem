//
//  testSigSegv.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 08/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <stdlib.h> /* for abort, exit */
#include <signal.h>
#include <setjmp.h>

jmp_buf jumpLabel;

void ErrorDetected (int signum)
{
   longjmp(jumpLabel, 1);
}

struct GameObject {
    int number;
};

int main() {

    signal(SIGSEGV, ErrorDetected);
        
    switch (setjmp (jumpLabel)) {
        case 0: {
            
            GameObject* object = 0;//new GameObject();
            object->number = 5;
            std::cout << "Ended"<< std::endl;
            
            break;
        }
        case 1: {
            
            std::cout << "Scripts crashed!"<< std::endl;
        }
    }
    
    return 0;
}
