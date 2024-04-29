#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

class GameViewWindow {
public:
    static bool isPlaying;

public:
    static void Imgui();

    static ImVec2 GetLargestSizeForViewport();

    static ImVec2 GetCenterPositionForViewport(ImVec2);

};


