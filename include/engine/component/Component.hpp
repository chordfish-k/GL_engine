#pragma once

class GameObject;

class Component {
public:
    GameObject *gameObject = nullptr;

protected:
    Component() {};

public:
    virtual void Start() {};
    virtual void Update(float dt) = 0;
};