//
//  GameComponent.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

class GameComponent {
private:
    static int componentCounter;
public:
    const static int NumberOfComponents() {
        return componentCounter;
    }

    template<typename T>
    static int GetComponentID() {
        static int componentID = componentCounter++;
        return componentID;
    }

};