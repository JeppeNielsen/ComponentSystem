//
//  unitTests.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 18/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Logic.hpp"
#include "UnitTest.hpp"





int main_unittest() {

    UnitTest::Run("CreateObject" , [] () {
        GameWorld world;
        world.CreateObject();
        return world.ObjectCount() == 1;
    });

    UnitTest::Run("RemoveObject" , [] () {
        GameWorld world;
        auto object = world.CreateObject();
        bool wasOne = world.ObjectCount() == 1;
        object->Remove();
        world.Update(1);
        return wasOne && world.ObjectCount() == 0;
    });
    
    UnitTest::Run("GameWorld::Clear" , [] () {
        GameWorld world;
        world.CreateObject();
        bool wasOne = world.ObjectCount() == 1;
        world.Clear();
        return wasOne && world.ObjectCount() == 0;
    });

    UnitTest::Run("Parenting" , [] () {
        GameWorld world;
        auto parent = world.CreateObject();
        auto child = world.CreateObject();
        child->Parent = parent;
        return parent->Children().size()==1 && child->Children().size()==0 && parent->Children()[0] == child;
    });

    UnitTest::Run("Unparenting" , [] () {
        GameWorld world;
        auto parent = world.CreateObject();
        auto child = world.CreateObject();
        child->Parent = parent;
        bool wasParented = parent->Children().size()==1 && child->Children().size()==0 && parent->Children()[0] == child;
        child->Parent = 0;
        return wasParented && parent->Children().size() == 0;
    });

    UnitTest::Run("System AddedObject" , [] () {
        GameWorld world;
        int objectsAddedToRenderSystem = 0;
        auto& renderSystem = world.GetSystem<RenderSystem>();
        renderSystem.Added.Bind([&](GameObject* o){ objectsAddedToRenderSystem++; });
        
        auto object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(1);
        
        return objectsAddedToRenderSystem == 1;
    });
    
    UnitTest::Run("System AddedObject 2" , [] () {
        GameWorld world;
        int objectsAddedToRenderSystem = 0;
        auto& renderSystem = world.GetSystem<RenderSystem>();
        renderSystem.Added.Bind([&](GameObject* o){ objectsAddedToRenderSystem++; });
        
        auto object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        
        auto object2 = world.CreateObject();
        object2->AddComponent<Transform>();
        
        world.Update(1);
        
        return objectsAddedToRenderSystem == 1;
    });
    
    UnitTest::Run("System RemovedObject" , [] () {
        GameWorld world;
        int objectsAddedToRenderSystem = 0;
        auto& renderSystem = world.GetSystem<RenderSystem>();
        renderSystem.Added.Bind([&](GameObject* o){ objectsAddedToRenderSystem++; });
        renderSystem.Removed.Bind([&](GameObject* o){ objectsAddedToRenderSystem--; });
        
        auto object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(1);
        bool wasOne = objectsAddedToRenderSystem == 1;
        object->RemoveComponent<Transform>();
        world.Update(1);
        return wasOne && objectsAddedToRenderSystem == 0;
    });
    
    
    
    
    

    return 0;
}