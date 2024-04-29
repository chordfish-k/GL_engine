#pragma once

#include "engine/core/GameObject.hpp"

class PropertiesWindow {
private:
    static GameObject * activeGameObject;

public:
    static void Imgui() {
        if (activeGameObject != nullptr) {
            ImGui::Begin("Properties");

            // TODO right click menu

            activeGameObject->Imgui();
            ImGui::End();
        }
    }

    static GameObject *GetActiveGameObject() {
        return activeGameObject;
    }

    static void SetActiveGameObject(GameObject *go) {
        activeGameObject = go;
    }
};
