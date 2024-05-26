#include <imgui.h>
#include "engine/physics2D/Angular.h"
#include "engine/editor/MyImGui.hpp"

Angular::Angular(float velocity, float damp)
    :velocity(velocity), damp(damp){

}

bool Angular::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Angular", ImGuiTreeNodeFlags_Selected)) {
        res = MyImGui::DrawFloatControl("velocity", velocity);
        res = res || MyImGui::DrawFloatControl("damp", damp);
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
