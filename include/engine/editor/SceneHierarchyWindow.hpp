#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/GameObject.hpp"
#include "engine/core/Window.hpp"
#include "PropertiesWindow.hpp"

class SceneHierarchyWindow {
public:
    static void Imgui() {
        ImGui::Begin("Scene Hierarchy");

        std::vector<Transform *> transforms = Window::GetScene()->root->children;

        int cnt = ShowSubNode(transforms, 0);

        ImGui::End();
    }

private:
    static int ShowSubNode(std::vector<Transform *> children, int indexStart) {
        auto base_flags = ImGuiTreeNodeFlags_DefaultOpen |
                          ImGuiTreeNodeFlags_FramePadding |
                          ImGuiTreeNodeFlags_OpenOnArrow |
                          ImGuiTreeNodeFlags_SpanAvailWidth;
        int index = indexStart;
        static int node_clicked = 1;

        for (auto obj : children) {
            auto go = obj->gameObject;
            if (!go->IsDoSerialization()) {
                continue;
            }
            ImGui::PushID(index);

            auto node_flags = base_flags;
            const bool is_selected = (node_clicked == index+1);
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;

            bool treeNodeOpen = ImGui::TreeNodeEx(
                (void*)(intptr_t)index,
                node_flags,
                go->name.c_str());

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                node_clicked = index+1;
                PropertiesWindow::SetActiveGameObject(go);
            }

            ImGui::PopID();

            ShowSubNode(obj->children, index + 1);

            if (treeNodeOpen) {
                ImGui::TreePop();
            }

            index += obj->children.size() + 1;
        }

        return index;
    }
};

