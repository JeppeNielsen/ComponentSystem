//
//  ComponentIDs.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 30/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

template<typename T>
int GetComponentID() {
    return 0;
}

struct Transform;
struct Velocity;
struct Renderable;
struct Touchable;

template<> int GetComponentID<Transform>() { return 0; }
template<> int GetComponentID<Velocity>() { return 1; }
template<> int GetComponentID<Renderable>() { return 2; }
template<> int GetComponentID<Touchable>() { return 3; }
