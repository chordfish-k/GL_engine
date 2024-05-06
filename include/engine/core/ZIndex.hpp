#pragma once

#include <imgui.h>
#include "engine/editor/MyImGui.hpp"
class ZIndex {
private:
    int zIndex = 0;

public:
    ZIndex(int z) : zIndex(z){}

    int GetZIndex() const { return zIndex; }

    void SetZIndex(int zIndex) { this->zIndex = zIndex; }

    ZIndex &operator=(const int &z) {
        zIndex = z;
        return *this;
    }

    void Imgui() {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("ZIndex")) {
            MyImGui::DrawIntControl("z-Index", zIndex);
            ImGui::TreePop();
        }
    }
};
