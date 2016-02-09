//
//  IScriptSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
struct GameObject;

struct IScriptSystem {
    virtual ~IScriptSystem() {}
    virtual void ObjectAdded(GameObject* object) = 0;
    virtual void ObjectRemoved(GameObject* object) = 0;
    virtual void Update(float dt) = 0;
    virtual int AddObject(GameObject* object) = 0;
    virtual GameObject* RemoveObject(int indexToRemove) = 0;
};
