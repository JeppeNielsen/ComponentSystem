//
//  GameObjectBase.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"

class GameObjectBase {
public:
    using Component = void*;
    Component* components;

    GameObjectBase() {
        components = new Component[GameComponent::NumberOfComponents()];
    }
    virtual ~GameObjectBase() { delete[] components; }

    template<typename T>
    T* AddComponent() {
        return (T*)addComponents->operator[](GameComponent::GetComponentID<T>())(this);
    }
    
    template<typename T>
    T* GetComponent() {
        return (T*)components[GameComponent::GetComponentID<T>()];
    }
    
protected:
    std::vector<std::function<void*(void*)>>* addComponents;
};