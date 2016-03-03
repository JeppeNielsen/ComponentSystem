//
//  GameComponent.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <iostream>
#include <typeinfo>

class IDHelper {
private:
    static int componentCounter;
    static int systemCounter;
public:
    const static int NumberOfComponents() {
        return componentCounter;
    }

    template<typename T>
    static int GetComponentID() {
        static int componentID = componentCounter++;
        std::cout << " type index  "<< componentID<<" : "<< typeid(T).name()<<std::endl;
        return componentID;
    }
    
    const static int NumberOfSystems() {
        return systemCounter;
    }
    
    template<typename T>
    static int GetSystemID() {
        static int systemID = systemCounter++;
        return systemID;
    }
};