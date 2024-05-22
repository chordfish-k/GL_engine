#include "engine/editor/GameViewWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/MainWindow.hpp"
#include <imgui.h>

bool GameViewWindow::isPlaying = false;
float GameViewWindow::leftX = 0;
float GameViewWindow::rightX = 0;
float GameViewWindow::topY = 0;
float GameViewWindow::bottomY = 0;

void GameViewWindow::Imgui() {
    bool open = true;
    ImGui::Begin("Game Viewport", &open,
                 ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_MenuBar);

    // 菜单
    ImGui::BeginMenuBar();
    if (ImGui::MenuItem("Play", "", isPlaying, !isPlaying)) {
        isPlaying = true;
//        EventSystem.notify(null, new Event(EventType.GameEngineStartPlay));
    }
    if (ImGui::MenuItem("Stop", "", !isPlaying, isPlaying)) {
        isPlaying = false;
//        EventSystem.notify(null, new Event(EventType.GameEngineStopPlay));
    }
    ImGui::EndMenuBar();

    ImGui::SetCursorPos(ImGui::GetCursorPos());
    ImVec2 windowSize = GetLargestSizeForViewport();
    ImVec2 windowPos = GetCenterPositionForViewport(windowSize);
    ImGui::SetCursorPos(windowPos);

    windowPos.x += ImGui::GetWindowPos().x;
    windowPos.y += ImGui::GetWindowPos().y;

    leftX = windowPos.x;
    bottomY = windowPos.y;
    rightX = windowPos.x + windowSize.x;
    topY = windowPos.y + windowSize.y;

    auto textureId = MainWindow::GetFrameBuffer()->GetTexture()->GetId();
    ImGui::Image((void*)(intptr_t)textureId,
                 {windowSize.x, windowSize.y},
                    {0,1}, {1, 0});

    MouseListener::SetGameViewportPos({windowPos.x, windowPos.y});
    MouseListener::SetGameViewportSize({windowSize.x, windowSize.y});

    ImGui::End();
}

ImVec2 GameViewWindow::GetLargestSizeForViewport() {
    ImVec2 windowSize = ImGui::GetContentRegionAvail(); // 获取可用的内容区域，不包括滚动的

    float aspectWidth = windowSize.x;
    float aspectHeight = aspectWidth / MainWindow::GetTargetAspectRatio();
    if (aspectHeight > windowSize.y) {
        // 贴合左右两边会超出高度，切换成贴合上下两边
        aspectHeight = windowSize.y;
        aspectWidth = aspectHeight * MainWindow::GetTargetAspectRatio();
    }
    return {aspectWidth, aspectHeight};
}

ImVec2 GameViewWindow::GetCenterPositionForViewport(ImVec2 aspectSize) {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    float viewportX = (windowSize.x / 2.0f)  - (aspectSize.x / 2.0f);
    float viewportY = (windowSize.y / 2.0f)  - (aspectSize.y / 2.0f);
//    util::Println(windowSize.x, ", ", aspectSize.x, ", ", viewportX);
//    util::Println(windowSize.y, ", ", aspectSize.y, ", ", viewportY);

    return {viewportX + ImGui::GetCursorPosX(),
                      viewportY + ImGui::GetCursorPosY()};
}

bool GameViewWindow::GetWantCaptureMouse() {
    return
            MouseListener::GetX() >= leftX && MouseListener::GetX() <= rightX &&
           MouseListener::GetY() >= bottomY && MouseListener::GetY() <= topY;
}
