#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/Camera.hpp"

#include <imgui.h>
#include <vector>
#include <fstream>

class GameObject;
//class Camera;
// class Renderer;

class AScene : public ASerializableObj{
protected:
    Renderer *renderer = nullptr;
    Camera *camera = nullptr;
    std::vector<GameObject *> gameObjects;

    bool sceneLoaded = false;

public:
    Transform *root = nullptr;

private:
    bool isRunning = false;

protected:
    AScene();

public:
    ~AScene();

    virtual void Init() {};

    void Start();

    void AddGameObject(GameObject *go);

    void RemoveAllGameObject();

    Camera *GetCamera();

    virtual void Update(float dt) = 0;

    void SceneImgui();

    virtual void Imgui();

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    void Save();

    void Load();

    std::vector<GameObject *> GetGameObjects();
};