#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/GameObject.hpp"
#include "engine/core/Window.hpp"

class SceneHierarchyWindow {
public:
    static void Imgui() {
        ImGui::Begin("Scene Hierarchy");

        std::vector<GameObject *> gameObjects = Window::GetScene()->GetGameObjects();

        int index = 0;
        for (auto &obj : gameObjects) {
            if (!obj->IsDoSerialization()) {
                continue;
            }
            ImGui::PushID(index);
            bool treeNodeOpen = ImGui::TreeNodeEx(
                obj->name.c_str(),
                ImGuiTreeNodeFlags_DefaultOpen |
                    ImGuiTreeNodeFlags_FramePadding |
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_SpanAvailWidth,
                obj->name.c_str());
            ImGui::PopID();

            if (treeNodeOpen) {
                ImGui::TreePop();
            }

            index++;
        }

        ImGui::End();
    }
};
