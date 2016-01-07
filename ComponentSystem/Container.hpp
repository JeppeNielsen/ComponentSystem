//
//  Container.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <assert.h>

template<typename Object>
class Container {
public:
    friend Object;
    
    struct ObjectInstance {
        Object object;
        int references;
        
        friend Object;
    };
    
    ObjectInstance* CreateObject() {
        TryGrow();
        ObjectInstance* instance = objects[size++];
        ++instance->references;
        return instance;
    }
    
    int Size() const { return size; }
    
    void RemoveObject(ObjectInstance* instance) {
        if ((--instance->references) == 0) {
            --size;
            std::swap(instance, objects[size]);
        }
    }
    
private:
    
    void TryGrow() {
        if (size<capacity) return;
        Grow((capacity + 20) * 2);
    }

    void Grow(int newCapacity) {
        objects.resize(newCapacity);
        for (int i=size; i<newCapacity; ++i) {
            objects[i] = new ObjectInstance();
            objects[i]->references = 0;
        }
        capacity = newCapacity;
    }
    
    using Objects = std::vector<ObjectInstance*>;
    Objects objects;
    
    int size;
    int capacity;
    
public:
    Container() : size(0), capacity(0) {}
    ~Container() {
        for (int i=0; i<objects.size(); ++i) {
            delete objects[i];
        }
        objects.clear();
    }
};