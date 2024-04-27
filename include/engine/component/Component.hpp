#pragma once

#include "engine/core/ASerializableObj.hpp"

class GameObject;

class Component : public ASerializableObj{
public:
    GameObject *gameObject = nullptr;
    std::string componentName = "Component";

public:
    Component() {};
    virtual void Start() {};
    virtual void Update(float dt) {};
    virtual void Imgui() {}
    virtual std::string Serialize() {return "";};
    virtual ASerializableObj *Deserialize(json j) {return nullptr;} ;
};

#define COMPONENT(name) \
    class name : public Component {\
        public:\
            std::string componentName=#name;
