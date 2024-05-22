#pragma once

#include <imgui.h>
#include "ImGuiFileDialog.h"
#include "engine/core/MainWindow.hpp"

class MenuBar {
public:
    static void Imgui() {
        ImGui::BeginMenuBar();

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
//                EventSystem.notify(null, new Event(EventType.SaveLevel));
                auto scene = MainWindow::GetScene();
                if (scene != nullptr) {
                    scene->Save();
                }
            }

            if (ImGui::MenuItem("Load Project", "Ctrl+L")) {
//                EventSystem.notify(null, new Event(EventType.LoadLevel));
                ProjectManagerWindow::shouldOpen = true;
            }

            if (ImGui::MenuItem("Close Project")) {
                //                EventSystem.notify(null, new Event(EventType.LoadLevel));
                Setting::PROJECT_ROOT = "";
                MainWindow::GetScene()->RemoveAllNodes();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
};
