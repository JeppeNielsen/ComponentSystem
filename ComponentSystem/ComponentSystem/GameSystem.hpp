//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <tuple>
#include <vector>
#include <map>

class GameWorld;
class GameObject;

class IGameSystem {
protected:
    virtual ~IGameSystem() {}
    
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;

public:
    const ObjectCollection& Objects() {
        return objects;
    }
    
protected:
    using MetaData = std::map<GameObject*, void*>;
    MetaData metadata;
    
    void SetMetaData(GameObject* object, void* data) {
        metadata[object] = data;
    }
    
    void* GetMetaData(GameObject* object) {
        return metadata[object];
    }
    
    virtual void Initialize(GameWorld* world) { }
    virtual void Update(float dt) { }
    virtual void Render() { }
    virtual void ObjectAdded(GameObject* object) {}
    virtual void ObjectRemoved(GameObject* object) {}
    
    int index;
    
    friend class GameObject;
    friend class GameWorld;
};


template<typename... ComponentList>
class GameSystem : public IGameSystem {
protected:
    GameSystem() { }
    virtual ~GameSystem() {}
    
    using Components = std::tuple<ComponentList*...>;
    
    //using Systems = meta::list<>;
    
    //void Initialize(GameWorld* world) {
        
    //}
    
    virtual void Initialize(GameWorld* world);
};

class GameConcept : public GameSystem<> {

};