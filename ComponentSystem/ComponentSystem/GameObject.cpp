//
//  GameObject.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 03/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include "GameWorld.hpp"

GameObject::GameObject() : world(0)  {
    int numberOfComponents = IDHelper::NumberOfComponents();
    components = new ComponentPtr[numberOfComponents];
    for(int i=0; i<numberOfComponents; ++i) {
        components[i] = 0;
    }
    
    Parent = 0;
    Parent.Changed.Bind([this]() {
        assert(Parent!=this);
        auto prevParent = Parent.PreviousValue();
        if (prevParent) {
            auto& children = prevParent->children;
            children.erase(std::find(children.begin(), children.end(), this));
        }
        
        if (Parent) {
            Parent()->children.push_back(this);
        }
    });
}

GameObject::~GameObject() { delete[] components; }

void GameObject::Remove() {
    if (isRemoved) return;
    isRemoved = true;
    
//        meta::for_each_in_tuple(world->components, [this] (auto& component) {
//            using ComponentType = meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>;
//            if (HasComponent<ComponentType>()) {
//                RemoveComponent<ComponentType>();
//            }
//        });
    
    world->removeActions.emplace_back([this]() {
        world->objects.RemoveObject(instance);
        isRemoved = false;
    });
    
    for(auto child : children) {
        child->Remove();
    }
}


void GameObject::Reset() {
    isRemoved = false;
    activeComponents = 0;
    removedComponents = 0;
#ifdef SCRIPTING_ENABLED
    for(int i=0; i<activeScriptComponents.size(); ++i) {
        activeScriptComponents[i]=false;
        removedScriptComponents[i]=false;
    }
#endif
}

void GameObject::SetWorld(GameWorld* w) {
    if (world) return;
    world = w;
#ifdef SCRIPTING_ENABLED
    InitializeScriptingData();
#endif
}

