#include <imgui.h>
#include "engine/script/Script.hpp"
#include "engine/editor/MyImGui.hpp"
#include "engine/util/Common.hpp"

bool Script::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        auto path = filePath;
        res = MyImGui::DrawResourceDragDropBox("script", path, "SCRIPT");
        if (res) {
            filePath = util::PathRelativeToProjectRoot(path);
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
