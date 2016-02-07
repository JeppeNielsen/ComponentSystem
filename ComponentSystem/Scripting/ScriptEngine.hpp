//
//  ScriptEngine.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 25/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include "ScriptClass.hpp"

class ScriptEngine {    
public:
    std::vector<std::string> Compile(const std::string& cmd);
    
    bool BuildDir(const std::string& dir);
    
    void WriteComponentIDsHeader(const std::string& path);
    
    bool Run();
    
private:
    ScriptClass scriptWorld;

};