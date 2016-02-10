//
//  ITypeInfo.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 09/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

struct ITypeInfo {
    virtual void SetInstance(void* instancePtr) = 0;
};

template<typename T>
struct ScriptTypeInfo : public ITypeInfo {

    void SetInstance(void* instancePtr) override {
        T* ptr = (T*)instancePtr;
        instance = *ptr;
    }
    
    

private:
    T instance;

};
