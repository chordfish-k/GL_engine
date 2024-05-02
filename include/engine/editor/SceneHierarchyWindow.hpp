#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/Window.hpp"
#include "PropertiesWindow.hpp"

class SceneHierarchyWindow {
public:
    static void Imgui() {
        ImGui::Begin("Scene Hierarchy");

        auto root = Window::GetScene()->root;
        auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                         ImGuiTreeNodeFlags_FramePadding |
                         ImGuiTreeNodeFlags_OpenOnArrow |
                         ImGuiTreeNodeFlags_SpanAvailWidth;
        int index = 0;
        ImGui::PushID(index);
        bool treeNodeOpen = ImGui::TreeNodeEx(
            (void*)(intptr_t)index,
            baseFlags,
            root->GetName().c_str());
        ImGui::PopID();

        if (treeNodeOpen) {
            // sub
            ShowSubNodes(root, index);
            ImGui::TreePop();
        }

        ImGui::End();
    }

    static int ShowSubNodes(Node *root, int indexStart) {
        static int selectIndex = 0;
        int index = indexStart;

        auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                         ImGuiTreeNodeFlags_FramePadding |
                         ImGuiTreeNodeFlags_OpenOnArrow |
                         ImGuiTreeNodeFlags_SpanAvailWidth;

        for (int i = 0; i < root->children.size(); i++) {
            auto obj = root->children[i];
            if (!obj->IsDoSerialization()) {
                continue;
            }
            ImGui::PushID(index);
            bool treeNodeOpen = ImGui::TreeNodeEx(
                (void*)(intptr_t)index,
                selectIndex == index ? (baseFlags | ImGuiTreeNodeFlags_Selected) : baseFlags,
                obj->GetName().c_str());
            ImGui::PopID();

            // select
            if (ImGui::IsItemClicked()) {
                selectIndex = index;
                PropertiesWindow::SetActiveNode(obj);
            }

            int subCnt = 0;

            if (treeNodeOpen) {
                // sub
                subCnt = ShowSubNodes(obj, index + 1);
                ImGui::TreePop();
            }

            index += subCnt + 1;
        }
        return index;
    }
};
