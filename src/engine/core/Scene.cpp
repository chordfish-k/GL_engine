#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/core/Camera.hpp"
#include "engine/node/Node.hpp"
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/editor/ImGuiFileDialog.h"
#include "engine/editor/FileDialog.hpp"

Scene::Scene(ASceneInitializer *sceneInitializer)
    :sceneInitializer(sceneInitializer) {
    renderer = new Renderer();
    root = new Node();
    sceneToolsRoot = new Node();
    sceneToolsRoot->SetDoSerialization(false);
    root->AddNode(sceneToolsRoot);
    root->name = "Root";
    isRunning = false;
}

Scene::~Scene() {
    delete renderer;
    renderer = nullptr;
    delete root;
    root = nullptr;
    delete sceneInitializer;
    sceneInitializer = nullptr;
}

void Scene::Init() {
    camera = new Camera({0,0});
    sceneInitializer->LoadResources(this);
    sceneInitializer->Init(this);
}

void Scene::Start() {
    root->Start();
    isRunning = true;
}

void Scene::Update(float dt) {
    root->Update(dt);
    root->CheckDelete();
    Render();
}

void Scene::AddNode(Node *n) const {
    root->AddNode(n);
    n->GeneratedId();
    if (isRunning) {
        n->Start();
    }
}

void Scene::AddNodeAsChild(Node *parent, Node *n) const {
    parent->AddNode(n);
    n->GeneratedId();
    if (isRunning) {
        n->Start();
    }
}

void Scene::RemoveAllNodes() const {
    root->RemoveAllNodes();
}

Camera *Scene::GetCamera() {
    return this->camera;
}

void Scene::SceneImgui() {
    Imgui();
}

void Scene::Imgui() {
    sceneInitializer->Imgui();
}

json Scene::Serialize() const {
    json j;
    j = root->Serialize();
    return j;
}

Node *Scene::Deserialize(json j) const {
    RemoveAllNodes();
    root->Deserialize(j);
    return root;
}

void Scene::Save() {
    auto path = sceneInitializer->GetFilePath();

    auto save = [&](const std::string &path) {
        util::Println("Save:", path);

        std::string jsonText = this->Serialize().dump(2);
        std::ofstream out(path, std::ios::trunc);
        if (out.is_open()){
            out << jsonText;
            out.close();
        } else {
            util::Println("ERROR:Saving Scene failed.");
        }
    };

    if (path.empty()) {
        FileDialog::DisplayWithCallback("save",
                                        [=](auto filePath, auto name){
            sceneInitializer->SetFilePath(name);
            save(name);
        }, "scene.scene", ".scene");
    } else {
        save(path);
    }
}

void Scene::Load() {
    auto path = sceneInitializer->GetFilePath();
    if (path == "") return;

    util::Println("Load:", path);

    std::ifstream fin(path);
    std::string jsonText;
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        jsonText = ss.str();
    }
    if (jsonText.empty()) return;

    root->Deserialize(Str2Json(jsonText));
}

void Scene::Destroy() {
    root->Destroy();
}

Renderer *Scene::GetRenderer() const {
    return renderer;
}

void Scene::Render() {
    this->renderer->Render();
}
Node *Scene::GetNodeByUid(int uid) {
    Node *node = nullptr;
    root->TravelOnSubTree([&](auto n){
        if (n->GetUid() == uid) {
            node = n;
            return;
        }
    });
    return node;
}
