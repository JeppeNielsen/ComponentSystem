//
//  Container.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <assert.h>

template<typename Object>
class Container;

template<typename Object>
class Handle {
private:
    Handle(Container<Object>* container, int index, int version)
    : container(container), index(index), version(version) {}

    int index;
    int version;
    Container<Object>* container;
    
    friend class Container<Object>;

    Handle() : index(0), version(0), container(0) {}
public:
    Handle(const Handle<Object>& handle) : container(handle.container), index(handle.index), version(handle.version) {}

    Object* operator->() {
        if (!IsValid()) return 0;
        return container->GetObject(index);
    }
    
    explicit operator bool() {
        return operator->();
    }
    
    bool IsValid() {
        if (!container) return false;
        return version == container->Version(index);
    }
    
    static Handle<Object> Empty() {
        return Handle();
    }
};

template<typename Object>
class Container {
public:
    friend Object;

    int CreateObject() {
        TryGrow();
        int freeIndex = size++;
        auto& d = objectDatas[freeIndex];
        ++d.references;
        return d.handleIndex;
    }
    
    int Size() const { return size; }

    Object* GetObject(int index) {
        return &objects[handleDatas[index].index];
    }
    
    void RemoveObject(int index) {
        auto& h = handleDatas[index];
        auto& d = objectDatas[h.index];
        --d.references;
        
        if (d.references == 0) {
            --size;
            std::swap(objects[h.index], objects[size]);
            std::swap(objectDatas[h.index], objectDatas[size]);
            h.index = size;
            ++h.version;
        }
    }
    
    Handle<Object> GetHandle(int index) {
        return Handle<Object>(this, index, handleDatas[index].version);
    }
    
    int Version(int index) {
        return handleDatas[index].version;
    }
    

private:
    
    void TryGrow() {
        if (size<capacity) return;
        Grow((capacity + 10) * 2);
    }

    void Grow(int newCapacity) {
        
        handleDatas.resize(newCapacity);
        objects.resize(newCapacity);
        objectDatas.resize(newCapacity);
        
        for (int i=size; i<newCapacity; ++i) {
            auto& h = handleDatas[i];
            h.index = i;
            h.version = 0;
            
            auto& d = objectDatas[i];
            d.references = 0;
            d.handleIndex = i;
        }
        capacity = newCapacity;
    }
    
    void Swap(int a, int b) {
        std::swap(objects[a], objects[b]);
        std::swap(objectDatas[a], objectDatas[b]);
        ++objectDatas[b].version;
    }
    
    struct HandleData {
        int index;
        int version;
    };
    
    struct ObjectData {
        int references;
        int handleIndex;
        bool alive() { return references>0; }
    };
    
    using HandleDatas = std::vector<HandleData>;
    using Objects = std::vector<Object>;
    using ObjectDatas = std::vector<ObjectData>;
    
    HandleDatas handleDatas;
    Objects objects;
    ObjectDatas objectDatas;
    
    int size;
    int capacity;
    
public:
    Container() : size(0), capacity(0) {}
};