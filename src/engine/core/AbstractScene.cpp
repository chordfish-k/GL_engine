#pragma once

#include "engine/core/AbstractScene.hpp"

void AbstractScene::Start() {
    for (auto go : gameObjects) {
        go->Start();
    }
    isRunning = true;
}

void AbstractScene::AddGameObject(GameObject *go) {
    gameObjects.push_back(go);
    if (isRunning) {
        go->Start();
    }
}
