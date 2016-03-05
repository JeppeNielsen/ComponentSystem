//
//  GameObject.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 03/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include "GameWorld.hpp"

GameObject::GameObject() : world(0)  {
    int numberOfComponents = IDHelper::NumberOfComponents();
    components = new ComponentPtr[numberOfComponents];
    for(int i=0; i<numberOfComponents; ++i) {
        components[i] = 0;
    }
    
    Parent = 0;
    Parent.Changed.Bind([this]() {
        assert(Parent!=this);
        auto prevParent = Parent.PreviousValue();
        if (prevParent) {
            auto& children = prevParent->children;
            children.erase(std::find(children.begin(), children.end(), this));
        }
        
        if (Parent) {
            Parent()->children.push_back(this);
        }
    });
}

GameObject::~GameObject() { delete[] components; delete[] systemIndices; }

void GameObject::Remove() {
    if (isRemoved) return;
    isRemoved = true;
    
    for(int i=0; i<world->components.size();++i) {
        RemoveComponent(i);
    }

    world->removeActions.emplace_back([this]() {
        world->objects.RemoveObject(instance);
        isRemoved = false;
    });
    
    for(auto child : children) {
        child->Remove();
    }
}


void GameObject::Reset() {
    isRemoved = false;
    activeComponents = 0;
    removedComponents = 0;
#ifdef SCRIPTING_ENABLED
    for(int i=0; i<activeScriptComponents.size(); ++i) {
        activeScriptComponents[i]=false;
        removedScriptComponents[i]=false;
    }
#endif
}

void GameObject::SetWorld(GameWorld* w) {
    if (world) return;
    world = w;
    systemIndices = new int[world->systems.size()];
#ifdef SCRIPTING_ENABLED
    InitializeScriptingData();
#endif
}

void GameObject::AddComponent(int componentID) {
    if (activeComponents[componentID]) return;
    world->addComponent[componentID](this);
}

void GameObject::RemoveComponent(int componentID) {
    if (!activeComponents[componentID]) return;
    world->removeComponent[componentID](this);
}

void GameObject::ToJson(std::ostream& stream, SerializePredicate predicate) {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    WriteJson(writer, predicate);
    writer.close();
}

void GameObject::WriteJson(minijson::object_writer& writer, SerializePredicate predicate) {

    minijson::object_writer gameObject = writer.nested_object("GameObject");
    minijson::array_writer components = gameObject.nested_array("Components");
    
    for(int i=0; i<world->components.size(); ++i) {
        if (!(predicate && !predicate(this, i))) {
            SerializeComponent(i, components, false, 0);
        }
    }
 
    components.close();
    
    if (!children.empty()) {
        minijson::array_writer children_object = gameObject.nested_array("Children");
        for(auto child : children) {
            if (predicate && !predicate(child, -1)) {
                continue;
            }
            GameObject* childSpecific = (GameObject*)child;
            minijson::object_writer child_object = children_object.nested_object();
            childSpecific->WriteJson(child_object, predicate);
            child_object.close();
        }
        children_object.close();
    }
    
    gameObject.close();
}

void GameObject::AddComponent(minijson::istream_context& context, std::string componentName) {
        
        int componentID;
        if (world->TryGetComponentIndex(componentName, componentID) && !activeComponents[componentID]) {
            auto type = world->getTypeComponent[componentID](this);
            type.Deserialize(context);
        } else {
            minijson::ignore(context);
        }
    
    /*
        bool addedAny = false;
        meta::for_each_in_tuple(world->serializableComponents, [this, &context, &componentName, &addedAny] (auto componentPointer) {
            if (addedAny) {
                return;
            }
            using ComponentType = std::remove_const_t< std::remove_pointer_t<decltype(componentPointer)> >;
            if (componentName == world->componentNames[Settings::template GetComponentID<ComponentType>()]) {
                if (!HasComponent<ComponentType>()) {
                    ComponentType* component = AddComponent<ComponentType>();
                    auto type = component->GetType();
                    type.Deserialize(context);
                    addedAny = true;
                } else {
                    std::cout<<"Only one component per type allowed per object"<<std::endl;
                    minijson::ignore(context);
                }
            }
        });
        if (!addedAny) {
            
        }
    }
*/

}


void GameObject::SerializeComponent(int componentID, minijson::array_writer& writer, bool isReference, std::string* referenceID ) {
    if (!activeComponents[componentID]) return;
    minijson::object_writer componentWriter = writer.nested_object();
    auto type = world->getTypeComponent[componentID](this);
    std::string& name = world->componentNames[componentID];
    if (!isReference) {
        minijson::object_writer jsonComponent = componentWriter.nested_object(name.c_str());
        type.Serialize(jsonComponent);
        jsonComponent.close();
    } else {
        std::string referenceName = name + ":ref";
        minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
        if (!referenceID) {
            jsonComponent.write("id", "");
        } else {
            jsonComponent.write("id", *referenceID);
        }
        jsonComponent.close();
    }
    componentWriter.close();
}


