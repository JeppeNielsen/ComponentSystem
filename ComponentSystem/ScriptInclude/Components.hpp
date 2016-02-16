//
//  Components.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 13/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

struct Transform {
    Transform() : x(0), y(0) {}

    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Renderable {
    int ImageNo;
};