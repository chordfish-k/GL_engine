#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/ASerializableObj.hpp"
#include "engine/core/ASceneInitializer.hpp"
#include <imgui.h>
#include <vector>
#include <fstream>

class Camera;
class Node;
//class Renderer;

class Scene {
public:
    Node *root = nullptr;
    Node *sceneToolsRoot = nullptr;

private:
    Renderer *renderer = nullptr;
    Camera *camera = nullptr;
    bool isRunning = false;
    ASceneInitializer *sceneInitializer = nullptr;

public:

    Scene(ASceneInitializer *sceneInitializer);

    ~Scene();

    virtual void Init();

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

    void Destroy();

    Renderer *GetRenderer() const;

    void Render();

    Node *GetNodeByUid(int uid);
};