#pragma once

#include "Camera.hpp"

class AbstractScene {
protected:
    Camera *camera;

public:
    AbstractScene() {}

    virtual void Update(float dt) = 0;

    virtual void Init() = 0;
};