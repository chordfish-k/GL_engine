#pragma once

// #include "engine/core/GameObject.hpp"
// #include "engine/core/Camera.hpp"
#include "engine/renderer/Renderer.hpp"
#include <vector>

class GameObject;
class Camera;
// class Renderer;

class AbstractScene {
protected:
    Renderer *renderer;
    Camera *camera;
    std::vector<GameObject *> gameObjects;

private:
    bool isRunning = false;

protected:
    AbstractScene();

public:
    ~AbstractScene();

    virtual void Init() {};

    void Start();

    void AddGameObject(GameObject *go);

    Camera *GetCamera();

    virtual void Update(float dt) = 0;
};