#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/ASerializableObj.hpp"
// #include "engine/core/Camera.hpp"

#include <imgui.h>
#include <vector>
#include <fstream>

class Camera;
class Node;
//class Renderer;

class AScene {
public:
    Node *root = nullptr;

protected:
    Renderer *renderer = nullptr;
    Camera *camera = nullptr;
    bool sceneLoaded = false;

private:
    bool isRunning = false;

protected:
    AScene();

public:
    ~AScene();

    virtual void Init() {};

    void Start();

    void AddNode(Node *n) const;

    void AddNodeAsChild(Node *parent, Node *n) const;

    void RemoveAllNodes() const;

    Camera *GetCamera();

    virtual void Update(float dt);

    void SceneImgui();

    virtual void Imgui();

    json Serialize() const;

    Node *Deserialize(json j) const;

    void Save();

    void Load();

    Renderer *GetRenderer() const;

};