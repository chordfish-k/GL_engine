#include <imgui.h>
#include <filesystem>
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/FileDialog.hpp"
#include "engine/util/Setting.hpp"
#include "engine/core/MainWindow.hpp"

bool ProjectManagerWindow::shouldOpen = false;

void ProjectManagerWindow::Imgui() {

    if (!shouldOpen && !Setting::PROJECT_ROOT.empty()) return;

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
            util::Println("New:", fileName);
            Setting::PROJECT_ROOT = fileName;
            ProjectManagerWindow::shouldOpen = false;
            // TODO 创建项目文件夹
            std::filesystem::create_directory(fileName);
        });
    }

    ImGui::SameLine();
    if (ImGui::Button("Load Project", ImVec2(buttonWidth, buttonHeight))) {
        FileDialog::DisplayWithCallback("Choose Dir",
                                        [](auto filePath, auto fileName)
        {
            util::Println("Load:", filePath);
            Setting::PROJECT_ROOT = filePath;
            ProjectManagerWindow::shouldOpen = false;
            auto scene = MainWindow::GetScene();
            if (scene) scene->RemoveAllNodes();
        });
    }

    ImGui::End();
}

