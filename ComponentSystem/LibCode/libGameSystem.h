//
//  libGameSystem.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "IScriptSystem.hpp"
#include <vector>

template<typename... T>
class GameSystem : public IScriptSystem {
protected:
    virtual void ObjectAdded(GameObject* object) override { }
    virtual void ObjectRemoved(GameObject* object) override { }
    virtual void Update(float dt) override {}
    
    virtual int AddObject(GameObject* object) override {
        int ret = (int)objects.size();
        objects.push_back(object);
        return ret;
    }
    
    virtual GameObject* RemoveObject(int indexToRemove) override {
        GameObject* lastObject = objects[objects.size()-1];
        objects[indexToRemove] = lastObject;
        objects.pop_back();
        return lastObject;
    }
    
private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    friend class GameObject;
protected:
    const ObjectCollection& Objects() {
        return objects;
    }
};
