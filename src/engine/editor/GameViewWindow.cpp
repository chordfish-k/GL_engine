#include "engine/editor/GameViewWindow.hpp"
#include "engine/core/Window.hpp"
#include <imgui.h>


void GameViewWindow::Imgui() {
    bool open = true;
    ImGui::Begin("Game Viewport", &open,
                 ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse);

    ImVec2 windowSize = GetLargestSizeForViewport();
    ImVec2 windowPos = GetCenterPositionForViewport(windowSize);

    ImGui::SetCursorPos({windowPos.x, windowPos.y});
    auto textureId = Window::GetFrameBuffer()->GetTexture()->GetId();
    ImGui::Image((void*)(intptr_t)textureId,
                 {windowSize.x, windowSize.y},
                    {0,1}, {1, 0});

    ImGui::End();
}

ImVec2 GameViewWindow::GetLargestSizeForViewport() {
    ImVec2 windowSize = ImGui::GetContentRegionAvail(); // 获取可用的内容区域，不包括滚动的

    windowSize.x -= ImGui::GetScrollX();
    windowSize.y -= ImGui::GetScrollY();

    float aspectWidth = windowSize.x;
    float aspectHeight = aspectWidth / Window::GetTargetAspectRatio();
    if (aspectHeight > windowSize.y) {
        // 贴合左右两边会超出高度，切换成贴合上下两边
        aspectHeight = windowSize.y;
        aspectWidth = aspectHeight * Window::GetTargetAspectRatio();
    }

    return ImVec2(aspectWidth, aspectHeight);
}

ImVec2 GameViewWindow::GetCenterPositionForViewport(ImVec2 aspectSize) {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    windowSize.x -= ImGui::GetScrollX();
    windowSize.y -= ImGui::GetScrollY();

    float viewportX = (windowSize.x / 2.0f)  - (aspectSize.x / 2.0f);
    float viewportY = (windowSize.y / 2.0f)  - (aspectSize.y / 2.0f);

    return ImVec2(viewportX + ImGui::GetCursorPosX(),
                      viewportY + ImGui::GetCursorPosY());
}
