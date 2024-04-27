#pragma once

#include "engine/core/ASerializableObj.hpp"

class GameObject;

class Component : public ASerializableObj{
public:
    GameObject *gameObject = nullptr;
    std::string componentName = "Unknown";

public:
    Component() {};
    virtual void Start() {};
    virtual void Update(float dt) {};
    virtual void Imgui() {}
};

#define COMPONENT(name) \
    class name : public Component {\
        public:\
            std::string componentName=#name;
