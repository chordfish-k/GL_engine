#pragma once

#include "engine/renderer/Renderer.hpp"
#include <imgui.h>
#include <vector>

class GameObject;
class Camera;
// class Renderer;

class AbstractScene {
protected:
    Renderer *renderer;
    Camera *camera;
    std::vector<GameObject *> gameObjects;
    GameObject *activeGameObject = nullptr;

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

    void SceneImgui();

    void Imgui();
};