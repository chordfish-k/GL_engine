#include "engine/core/ZIndex.hpp"

int ZIndex::GetZIndex() const { return zIndex; }

void ZIndex::SetZIndex(int zIndex) { this->zIndex = zIndex; }

ZIndex &ZIndex::operator=(const int &z) {
    zIndex = z;
    return *this;
}

bool ZIndex::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("ZIndex", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawIntControl("z-Index", zIndex);
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}