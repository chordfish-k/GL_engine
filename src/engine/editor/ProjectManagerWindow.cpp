#include <imgui.h>
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/FileDialog.hpp"

std::string ProjectManagerWindow::projectLocation;

void ProjectManagerWindow::Imgui() {

    if (!ProjectManagerWindow::projectLocation.empty()) return;

    ImGui::Begin("Projects", NULL,
                 ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_MenuBar |
                     ImGuiWindowFlags_NoDocking);
    int w = (int)ImGui::GetContentRegionAvail().x;
    int h = (int)ImGui::GetContentRegionAvail().y;
    // 计算按钮的宽度，高度
    float buttonWidth = (w - ImGui::GetStyle().ItemSpacing.x) / 2;
    float buttonHeight = (h - ImGui::GetStyle().ItemSpacing.y);

    if (ImGui::Button("New Project", ImVec2(buttonWidth, buttonHeight))) {
        FileDialog::DisplayWithCallback("Choose Dir",
                                        [](auto filePath, auto fileName)
        {
                                            Window::SetShowingTopWindow(false);
            util::Println("New:", filePath);
            projectLocation = filePath;
        });
    }

    ImGui::SameLine();
    if (ImGui::Button("Load Project", ImVec2(buttonWidth, buttonHeight))) {
        FileDialog::DisplayWithCallback("Choose Dir",
                                        [](auto filePath, auto fileName)
        {
                                            Window::SetShowingTopWindow(false);
            util::Println("Load:", filePath);
            projectLocation = filePath;
        });
    }

    ImGui::End();
}

