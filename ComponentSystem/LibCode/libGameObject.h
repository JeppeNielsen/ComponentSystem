#pragma once
struct Transform;
struct Touchable;
struct Renderable;
struct Velocity;
struct Button;

struct GameObject {
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0; 
    virtual void* AddScriptComponent(int componentID) = 0; 
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { return (T*)0; }
};
template<> Transform* GameObject::GetComponent<Transform>() { return (Transform*) GetComponent(0); }
template<> Touchable* GameObject::GetComponent<Touchable>() { return (Touchable*) GetComponent(1); }
template<> Renderable* GameObject::GetComponent<Renderable>() { return (Renderable*) GetComponent(2); }
template<> Velocity* GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(3); }
template<> Transform* GameObject::AddComponent<Transform>() { return (Transform*) AddComponent(0); }
template<> Touchable* GameObject::AddComponent<Touchable>() { return (Touchable*) AddComponent(1); }
template<> Renderable* GameObject::AddComponent<Renderable>() { return (Renderable*) AddComponent(2); }
template<> Velocity* GameObject::AddComponent<Velocity>() { return (Velocity*) AddComponent(3); }
template<> Button* GameObject::GetComponent<Button>() { return (Button*) GetScriptComponent(0); }
template<> Button* GameObject::AddComponent<Button>() { return (Button*) AddScriptComponent(0); }
