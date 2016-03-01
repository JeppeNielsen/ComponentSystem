//
//  GameObject.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "IDHelper.hpp"

class GameObject {
protected:
    using Component = void*;
    Component* components;

    GameObject() {
        int numberOfComponents = IDHelper::NumberOfComponents();
        components = new Component[numberOfComponents];
        for(int i=0; i<numberOfComponents; ++i) {
            components[i] = 0;
        }
    }
    virtual ~GameObject() { delete[] components; }
    
    std::vector<std::array<std::function<void*(void*)>,2>>* commands;

public:
    template<typename T>
    T* GetComponent() {
        return (T*)components[IDHelper::GetComponentID<T>()];
    }

    template<typename T>
    T* AddComponent() {
        return (T*)commands->operator[](IDHelper::GetComponentID<T>())[0](this);
    }
    
    template<typename T>
    void RemoveComponent() {
        commands->operator[](IDHelper::GetComponentID<T>())[1](this);
    }
    
    virtual void Remove() = 0;
    
    using SerializePredicate = std::function<bool(GameObject*, int)>;
    
    virtual void ToJson(std::ostream& stream, SerializePredicate predicate = 0) = 0;
};