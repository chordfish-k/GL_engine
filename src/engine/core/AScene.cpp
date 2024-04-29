#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/AScene.hpp"

AScene::AScene() {
    renderer = new Renderer();
}

AScene::~AScene() {
    delete renderer;
    renderer = nullptr;
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
        j["gameObjects"][i] = gameObjects[i]->Serialize();
    }
    return j;
}

ASerializableObj *AScene::Deserialize(json j) {
    RemoveAllGameObject();
    auto &gos = j["gameObjects"];
    for (auto &go : gos) {
        auto *goImpl = new GameObject();
        gameObjects.push_back((GameObject*)goImpl->Deserialize(go));
    }
    return this;
}

void AScene::Save() {
    {
        std::string jsonText = Serialize().dump(2);
        std::ofstream out("scene.txt", std::ios::trunc);
        if (out.is_open()){
            out << jsonText;
            out.close();
        } else {
            util::Println("ERROR:Save Scene failed.");
        }
    }
}

void AScene::Load() {
    std::ifstream fin("scene.txt");
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
