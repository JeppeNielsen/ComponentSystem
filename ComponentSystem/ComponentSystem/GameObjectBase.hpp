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
protected:
    using Component = void*;
    Component* components;

    GameObjectBase() {
        int numberOfComponents = GameComponent::NumberOfComponents();
        components = new Component[numberOfComponents];
        for(int i=0; i<numberOfComponents; ++i) {
            components[i] = 0;
        }
    }
    virtual ~GameObjectBase() { delete[] components; }
    
    std::vector<std::array<std::function<void*(void*)>,2>>* commands;

public:
    template<typename T>
    T* GetComponent() {
        return (T*)components[GameComponent::GetComponentID<T>()];
    }

    template<typename T>
    T* AddComponent() {
        return (T*)commands->operator[](GameComponent::GetComponentID<T>())[0](this);
    }
    
    template<typename T>
    void RemoveComponent() {
        commands->operator[](GameComponent::GetComponentID<T>())[1](this);
    }
    
    virtual void Remove() = 0;
};