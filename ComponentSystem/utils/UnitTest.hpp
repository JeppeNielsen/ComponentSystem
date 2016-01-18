//
//  UnitTest.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 18/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include <string>
#include <iostream>

class UnitTest {
public:
    using Test = std::function<bool()>;
    static void Run(std::string name, Test test) {
        if (test()) {
            std::cout<< name << ": SUCCES"<<std::endl;
        } else {
            std::cout<< name << ": FAILED"<<std::endl;
        }
    }
};