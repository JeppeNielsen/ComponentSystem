//
//  GameWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 02/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include "GameWorld.hpp"

GameObject* GameWorld::CreateObject() {
    auto object = objects.CreateObject();
    object->object.instance = object;
    object->object.world = this;
    object->object.Reset();
    return &object->object;
}

void GameWorld::InitializeSystemBitsets() {
    int i = 0;
    meta::for_each_in_tuple(systems, [&](auto system) {
        meta::for_each_in_tuple(meta::Iterator<typename decltype(system)::Components>{}.Iterate(), [&,this](auto component) {
            systemBitsets[i][Settings::template GetComponentID<decltype(component)>()] = true;
        });
        ++i;
    });
}

GameWorld::GameWorld() {
    InitializeSystemBitsets();
}

void GameWorld::Initialize() {
    meta::for_each_in_tuple(addedToWorldSystems, [&](auto system) {
        std::get<
            Settings::template GetSystemID<
                std::remove_pointer_t<decltype(system)>
            >()
        >(systems).AddedToWorld(*this);
    });
}

void GameWorld::Update(float dt) {
    DoActions(createActions);
    DoActions(removeActions);
    meta::for_each_in_tuple(updateSystems, [this, dt] (auto system) {
        std::get<
                Settings::template GetSystemID<
                std::remove_pointer_t< decltype(system)>
                >()
            >(systems).Update(dt);
    });
}

void GameWorld::DoActions(Actions& list) {
   for(int i=0; i<list.size(); ++i) {
        list[i]();
   }
   list.clear();
}

int GameWorld::ObjectCount() {
    return objects.Size();
}

void GameObject::Reset() {
    isRemoved = false;
    activeComponents.reset();
    removedComponents.reset();
}

void GameObject::Remove() {
    if (isRemoved) return;
    isRemoved = true;
    
    meta::for_each_in_tuple(world->components, [this] (auto& component) {
        RemoveComponent<meta::mp_rename<std::remove_const_t<std::remove_reference_t<decltype(component)>>, meta::ReturnContainedType>>();
    });
    
    world->removeActions.emplace_back([this]() {
        world->objects.RemoveObject(instance);
        isRemoved = false;
    });
}



