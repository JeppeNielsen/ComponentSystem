//
//  testScriptParsing.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "ScriptParser.hpp"
#include "ScriptEngine.hpp"
#include "Timer.hpp"

int main_parsing() {

    /*
    ScriptClass scriptClasses;
    scriptClasses.name = "Root";

    ScriptParser parser;
    
    parser.ParseCode(scriptClasses,
    "/Projects/ComponentSystem/ComponentSystem/LibCode/ScriptTest.hpp",
    {
        "-I/Projects/ComponentSystem/ComponentSystem/LibCode",
        "-I/Projects/ComponentSystem/ComponentSystem/Scripting"
    });

    scriptClasses.Print();
    
    ScriptClass scriptWorld = parser.ExtractSystemsAndComponents(scriptClasses);
    
    parser.CreateScriptHeader(scriptWorld, "/Projects/ComponentSystem/ComponentSystem/LibCode/ScriptEngine_autoGenerated.cpp");
    */
     
    ScriptEngine engine;
    
    Timer timer;
    timer.Begin();
    engine.BuildDir("");
    double buildTime = timer.End();
    std::cout << "Build complete, time = " << buildTime <<"s"<< std::endl;
    
    engine.Run();
    
    
    

    return 0;
}