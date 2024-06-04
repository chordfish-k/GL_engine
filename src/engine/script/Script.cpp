#include <utility>
#include <imgui.h>
#include "engine/script/Script.hpp"
#include "engine/editor/MyImGui.hpp"

bool Script::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("ZIndex", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawResourceDragDropBox("script", filePath, "SCRIPT");
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
