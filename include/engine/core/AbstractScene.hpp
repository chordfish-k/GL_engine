#pragma once

#include "GameObject.hpp"
#include "engine/core/Camera.hpp"
#include <vector>

class AbstractScene {
protected:
    Camera *camera;
    std::vector<GameObject *> gameObjects;

private:
    bool isRunning = false;

protected:
    AbstractScene() {}

public:
    virtual void Init() {};

    void Start();

    void AddGameObject(GameObject *go);

    virtual void Update(float dt) = 0;
};