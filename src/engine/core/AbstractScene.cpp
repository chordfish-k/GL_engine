#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/AbstractScene.hpp"

AbstractScene::AbstractScene() {
    renderer = new Renderer();
}

AbstractScene::~AbstractScene() {
    delete renderer;
    renderer = nullptr;
}

void AbstractScene::Start() {
    for (auto go : gameObjects) {
        go->Start();
        renderer->Add(go);
    }
    isRunning = true;
}

void AbstractScene::AddGameObject(GameObject *go) {
    gameObjects.push_back(go);
    if (isRunning) {
        go->Start();
    }
}

Camera *AbstractScene::GetCamera() {
    return this->camera;
}

void AbstractScene::SceneImgui() {
    if (activeGameObject != nullptr) {
        ImGui::Begin("Inspector");
        activeGameObject->Imgui();
        ImGui::End();
    }
    Imgui();
}

void AbstractScene::Imgui() {}