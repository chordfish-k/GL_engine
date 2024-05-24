#include "engine/editor/MenuBar.hpp"

#include <imgui.h>
#include "engine/event/EventSystem.hpp"

void MenuBar::Imgui() {
    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            EventSystem::Notify(nullptr, {EventType::SaveScene});
        }

        if (ImGui::MenuItem("Load Project", "Ctrl+L")) {
            EventSystem::Notify(nullptr, {EventType::LoadScene});
        }

        if (ImGui::MenuItem("Close Project")) {
            EventSystem::Notify(nullptr, {EventType::CloseProject});
        }

        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}