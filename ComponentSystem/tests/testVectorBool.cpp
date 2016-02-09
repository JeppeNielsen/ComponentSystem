//
//  testVectorBool.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 08/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <bitset>
#include <iostream>

using Bitset = std::vector<bool>;

int main_vectorBool() {

    std::bitset<4> staticBits;
    staticBits[1]=true;
    staticBits[2]=true;
    
    Bitset set1 { true, false, true, false };
    Bitset set2 { true, false, true, false };
    
   // auto result = set1 & set2;
    
    //std::cout << result << std::endl;

    return 0;
}