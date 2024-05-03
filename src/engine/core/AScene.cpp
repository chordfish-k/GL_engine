#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/AScene.hpp"
#include "engine/node/Node.hpp"

AScene::AScene() {
    renderer = new Renderer();
    root = new Node();
    sceneToolsRoot = new Node();
    sceneToolsRoot->SetDoSerialization(false);
    root->AddNode(sceneToolsRoot);
    root->name = "Root";
}

AScene::~AScene() {
    delete renderer;
    renderer = nullptr;
    delete root;
    root = nullptr;
}

void AScene::Start() {
    if (!root) return;
    root->Start();
    isRunning = true;
}

void AScene::Update(float dt) {
    root->Update(dt);
    root->CheckDelete();
}

void AScene::AddNode(Node *n) const {
    root->AddNode(n);
    if (isRunning) {
        n->Start();
    }
}

void AScene::AddNodeAsChild(Node *parent, Node *n) const {
    parent->AddNode(n);
    if (isRunning) {
        n->Start();
    }
}

void AScene::RemoveAllNodes() const {
    root->RemoveAllNodes();
}

Camera *AScene::GetCamera() {
    return this->camera;
}

void AScene::SceneImgui() {
    Imgui();
}

void AScene::Imgui() {}

json AScene::Serialize() const {
    json j;
    j = root->Serialize();
    return j;
}

Node *AScene::Deserialize(json j) const {
    RemoveAllNodes();
    root->Deserialize(j);
    return root;
}

void AScene::Save() {
    {
        std::string jsonText = Serialize().dump(2);
        std::ofstream out("scene.json", std::ios::trunc);
        if (out.is_open()){
            out << jsonText;
            out.close();
        } else {
            util::Println("ERROR:Save Scene failed.");
        }
    }
}

void AScene::Load() {
    std::ifstream fin("scene.json");
    std::string jsonText;
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        jsonText = ss.str();
    }
    if (jsonText.empty()) return;

    root->Deserialize(Str2Json(jsonText));
    sceneLoaded = true;
}

Renderer *AScene::GetRenderer() const {
    return renderer;
}

void AScene::Render() {
    this->renderer->Render();
}

