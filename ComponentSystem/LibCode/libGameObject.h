#pragma once
struct Transform;
struct Touchable;
struct Renderable;
struct Velocity;
struct Button;
struct Sprite;

class GameObject {
private:
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0; 
    virtual void* AddScriptComponent(int componentID) = 0; 
    virtual void RemoveScriptComponent(int componentID) = 0; 
public:
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { return (T*)0; }
    template<typename T> void RemoveComponent() { }
};
template<> Transform* GameObject::GetComponent<Transform>() { return (Transform*) GetComponent(0); }
template<> Touchable* GameObject::GetComponent<Touchable>() { return (Touchable*) GetComponent(1); }
template<> Renderable* GameObject::GetComponent<Renderable>() { return (Renderable*) GetComponent(2); }
template<> Velocity* GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(3); }
template<> Transform* GameObject::AddComponent<Transform>() { return (Transform*) AddComponent(0); }
template<> Touchable* GameObject::AddComponent<Touchable>() { return (Touchable*) AddComponent(1); }
template<> Renderable* GameObject::AddComponent<Renderable>() { return (Renderable*) AddComponent(2); }
template<> Velocity* GameObject::AddComponent<Velocity>() { return (Velocity*) AddComponent(3); }
template<> void GameObject::RemoveComponent<Transform>() { RemoveComponent(0); }
template<> void GameObject::RemoveComponent<Touchable>() { RemoveComponent(1); }
template<> void GameObject::RemoveComponent<Renderable>() { RemoveComponent(2); }
template<> void GameObject::RemoveComponent<Velocity>() { RemoveComponent(3); }
template<> Button* GameObject::GetComponent<Button>() { return (Button*) GetScriptComponent(0); }
template<> Button* GameObject::AddComponent<Button>() { return (Button*) AddScriptComponent(0); }
template<> void GameObject::RemoveComponent<Button>() { RemoveScriptComponent(0); }
template<> Sprite* GameObject::GetComponent<Sprite>() { return (Sprite*) GetScriptComponent(1); }
template<> Sprite* GameObject::AddComponent<Sprite>() { return (Sprite*) AddScriptComponent(1); }
template<> void GameObject::RemoveComponent<Sprite>() { RemoveScriptComponent(1); }
