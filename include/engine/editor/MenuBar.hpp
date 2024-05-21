#pragma once

#include <imgui.h>
#include "ImGuiFileDialog.h"
class MenuBar {
public:
    static void Imgui() {
        ImGui::BeginMenuBar();

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
//                EventSystem.notify(null, new Event(EventType.SaveLevel));
                auto scene = Window::GetScene();
                if (scene != nullptr) {
                    scene->Save();
                }
            }

            if (ImGui::MenuItem("Load", "Ctrl+O")) {
//                EventSystem.notify(null, new Event(EventType.LoadLevel));
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
};
