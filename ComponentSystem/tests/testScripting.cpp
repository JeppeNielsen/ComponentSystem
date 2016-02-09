//
//  testScripting.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 25/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "ScriptEngine.hpp"
#include "Timer.hpp"

int main() {

    ScriptEngine engine;
    
    Timer timer;
    timer.Begin();
    engine.BuildDir("");
    double buildTime = timer.End();
    std::cout << "Build complete, time = " << buildTime <<"s"<< std::endl;
    
    engine.Run();
    
    return 0;
}
