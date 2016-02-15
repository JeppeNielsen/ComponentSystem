struct Velocity;
struct Transform;
struct Renderable;
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
template<> Velocity* GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(0); }
template<> Transform* GameObject::GetComponent<Transform>() { return (Transform*) GetComponent(1); }
template<> Renderable* GameObject::GetComponent<Renderable>() { return (Renderable*) GetComponent(2); }
template<> Velocity* GameObject::AddComponent<Velocity>() { return (Velocity*) AddComponent(0); }
template<> Transform* GameObject::AddComponent<Transform>() { return (Transform*) AddComponent(1); }
template<> Renderable* GameObject::AddComponent<Renderable>() { return (Renderable*) AddComponent(2); }
template<> void GameObject::RemoveComponent<Velocity>() { RemoveComponent(0); }
template<> void GameObject::RemoveComponent<Transform>() { RemoveComponent(1); }
template<> void GameObject::RemoveComponent<Renderable>() { RemoveComponent(2); }
template<> Sprite* GameObject::GetComponent<Sprite>() { return (Sprite*) GetScriptComponent(0); }
template<> Sprite* GameObject::AddComponent<Sprite>() { return (Sprite*) AddScriptComponent(0); }
template<> void GameObject::RemoveComponent<Sprite>() { RemoveScriptComponent(0); }
#include "TypeInfo.hpp"
#include "Components.hpp"
#include "ScriptExample.hpp"

extern "C" int CountSystems() {
   return 2;
}
extern "C" IScriptSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new MoverSystem();
      case 1: return new SpriteSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IScriptSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 1;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new Sprite();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Sprite*)component); break; }
   }
}

extern "C" TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      Sprite* component = (Sprite*)componentPtr;
      TypeInfo* info = new TypeInfo();
      info->AddField(component->width, "width");
      info->AddField(component->height, "height");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(TypeInfo* typeInfo) {
delete typeInfo;
}
