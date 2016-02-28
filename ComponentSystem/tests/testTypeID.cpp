//
//  testTypeID.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>

struct Renderable {
    int imageNo;
};

struct Transform {
    int x;
    int y;
};

struct Touchable {
    int clickThrough;
};

static int componentIDCounter = 0;

template<typename T>
int GetComponentID() {
    static int value = componentIDCounter++;
    return value;
}


struct TouchSystem {

    int fullIndex;

    void Initialize() {
        fullIndex += GetComponentID<Touchable>() + GetComponentID<Transform>();
    }
};




int main() {

    TouchSystem s;
    s.Initialize();

    std::cout << GetComponentID<Renderable>() << std::endl;
    std::cout << GetComponentID<Transform>() << std::endl;
    std::cout << GetComponentID<Renderable>() << std::endl;
    std::cout << GetComponentID<Transform>() << std::endl;
    
    
    return 0;



};