#pragma once

#include <imgui.h>
#include "engine/editor/MyImGui.hpp"
class ZIndex {
private:
    int zIndex = 0;

public:
    ZIndex(int z) : zIndex(z){}

    int GetZIndex() const;

    void SetZIndex(int zIndex);

    ZIndex &operator=(const int &z);

    bool Imgui();
};
