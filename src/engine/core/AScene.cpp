#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/AScene.hpp"
#include "engine/core/Window.hpp"

AScene::AScene() {
    renderer = new Renderer();
    root = new Transform();
}

AScene::~AScene() {
    delete renderer;
    delete root;
    RemoveAllGameObject();
}

void AScene::Start() {
    for (auto go : gameObjects) {
        go->Start();
        renderer->Add(go);
    }
    isRunning = true;
}

void AScene::AddGameObject(GameObject *go) {
    gameObjects.push_back(go);
    if (isRunning) {
        go->Start();
    }
}

void AScene::RemoveAllGameObject() {
    for (auto go : gameObjects) {
        delete go;
    }
}

Camera *AScene::GetCamera() {
    return this->camera;
}

void AScene::SceneImgui() {
//    if (activeGameObject != nullptr) {
//        ImGui::Begin("Inspector");
//        activeGameObject->Imgui();
//        ImGui::End();
//    }
    Imgui();
}

void AScene::Imgui() {}

json AScene::Serialize() {
    json j;
    for (int i = 0; i < gameObjects.size(); ++i) {
        if (gameObjects[i]->IsDoSerialization())
            j["gameObjects"][i] = gameObjects[i]->Serialize();
    }
    return j;
}

ASerializableObj *AScene::Deserialize(json j) {
    RemoveAllGameObject();

    std::vector<Component *> components;

    auto &gos = j["gameObjects"];
    for (auto &go : gos) {
        auto *goImpl = new GameObject();
        AddGameObject((GameObject*)goImpl->Deserialize(go));
        auto comp = goImpl->GetComponents();
        components.insert(components.end(), comp.begin(), comp.end());
    }

    std::sort(components.begin(), components.end(), [](auto &a, auto &b) {
        return a->GetUid() < b->GetUid();
    });

    // 构建Transform树
    for (auto go : gameObjects) {
        auto tr = go->transform;
        if (tr->parentUid == -1) {
            Window::GetScene()->root->children.push_back(tr);
        }
        for (auto id : tr->childrenUid) {
            if (components[id]->GetComponentName() != "Transform")
                continue;
            go->AddChild(components[id]->gameObject);

            ((Transform*)components[id])->parent = tr;
        }
    }
    return this;
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

    Deserialize(Str2Json(jsonText));
    sceneLoaded = true;
}

std::vector<GameObject *> AScene::GetGameObjects() {
    return gameObjects;
}


