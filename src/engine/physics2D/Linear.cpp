#include <imgui.h>
#include "engine/physics2D/Linear.hpp"
#include "engine/editor/MyImGui.hpp"

Linear::Linear(glm::vec2 velocity, float damp)
    :velocity(velocity), damp(damp){

}

bool Linear::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Linear", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawVec2Control("velocity", velocity, 0, 0.5f);
        res = res || MyImGui::DrawFloatControl("damp", damp);
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
