#pragma once

class AbstractScene {
public:
    AbstractScene() {}

    virtual void Update(float dt) = 0;

    virtual void Init() = 0;
};