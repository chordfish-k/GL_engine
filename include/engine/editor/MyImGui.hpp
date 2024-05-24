#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <imgui.h>
#include <glm/vec4.hpp>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include "engine/util/Print.hpp"
#include "engine/editor/FileSystemWindow.hpp"

#define COLUMN_WIDTH 80

class MyImGui {
public:

    static bool DrawIntSpinner(const std::string& label, int &values, int v_min=INT32_MIN, int v_max=INT32_MAX) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        int v = values;
        res = ImGui::InputInt("##dragInt", &v);
        if (v >= v_min && v <= v_max)
            values = v;

        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    static bool DrawIntControl(const std::string& label, int &values, int v_speed = 1, int v_min=0, int v_max=0) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        res = ImGui::DragInt("##dragInt", &values, (float)v_speed, v_min, v_max);
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    static bool DrawFloatControl(const std::string& label, float &values) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        res = ImGui::DragFloat("##dragFloat", &values, 0.1f);
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    static bool DrawCheckbox(const std::string& label, bool &values) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        res = ImGui::Checkbox("##checkbox", &values);

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    static bool DrawVec2Control(const std::string& label, glm::vec2 &values, float resetValue = 0.f, float speed = 0.1f, float columnWidth = COLUMN_WIDTH) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 3.f});

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
        ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);
        float widthEach = (ImGui::GetContentRegionAvail().x -2.f - buttonSize.x * 2.0f) / 2.0f;

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("X", {buttonSize.x, buttonSize.y})) {
            values.x = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesX = values.x;
        res = ImGui::DragFloat("##x", &vecValuesX, speed) || res;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("Y", {buttonSize.x, buttonSize.y})) {
            values.y = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesY = values.y;
        res = ImGui::DragFloat("##y", &vecValuesY, speed) || res;
        ImGui::PopItemWidth();

        values.x = vecValuesX;
        values.y = vecValuesY;

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();

        return res;
    }

    static bool DrawVec3Control(const std::string& label, glm::vec3 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 3.f});

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
        ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);
        float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 3.0f) / 3.0f;

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("X", {buttonSize.x, buttonSize.y})) {
            values.x = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesX = values.x;
        res = ImGui::DragFloat("##x", &vecValuesX, speed) || res;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("Y", {buttonSize.x, buttonSize.y})) {
            values.y = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesY = values.y;
        res = ImGui::DragFloat("##y", &vecValuesY, speed) || res;
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.15f, 0.1f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.2f, 0.2f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.15f, 0.1f, 0.8f, 1.0f});
        if (ImGui::Button("Z", {buttonSize.x, buttonSize.y})) {
            values.z = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesZ = values.z;
        res = ImGui::DragFloat("##z", &vecValuesZ, speed) || res;
        ImGui::PopItemWidth();

        values.x = vecValuesX;
        values.y = vecValuesY;
        values.z = vecValuesZ;

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();

        return res;
    }

    static bool DrawVec4Control(const std::string& label, glm::vec4 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 3.f});

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
        ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);
        float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 2.0f) / 2.0f;

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.2f, 1.0f});

        if (ImGui::Button("X", {buttonSize.x, buttonSize.y})) {
            values.x = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesX = values.x;
        res = ImGui::DragFloat("##x", &vecValuesX, speed) || res;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("Y", {buttonSize.x, buttonSize.y})) {
            values.y = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesY = values.y;
        res = ImGui::DragFloat("##y", &vecValuesY, speed) || res;
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.15f, 0.1f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.2f, 0.2f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.15f, 0.1f, 0.8f, 1.0f});
        if (ImGui::Button("Z", {buttonSize.x, buttonSize.y})) {
            values.z = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesZ = values.z;
        res = ImGui::DragFloat("##z", &vecValuesX, speed) || res;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushItemWidth(widthEach);
        ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.7f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.8f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.7f, 0.15f, 1.0f});
        if (ImGui::Button("W", {buttonSize.x, buttonSize.y})) {
            values.w = resetValue;
            res = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        float vecValuesW = values.w;
        res = ImGui::DragFloat("##w", &vecValuesZ, speed) || res;
        ImGui::PopItemWidth();

        values.x = vecValuesX;
        values.y = vecValuesY;
        values.z = vecValuesZ;
        values.w = vecValuesW;

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();

        return res;
    }

    static bool DrawColor4Control(const std::string& label, glm::vec4 &color) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);


        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        if (ImGui::ColorEdit4("##colorPicker", glm::value_ptr(color))) {
            res = true;
        }
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    static bool DrawResourceDragDropBox(const std::string &label, std::string &path) {
        bool res = false;
        auto lb = label.c_str();
        ImGui::PushID(lb);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, COLUMN_WIDTH);
        ImGui::Text("%s", lb);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;

        std::filesystem::path path1 = path;
        std::string name = path1.filename().string();
        if (ImGui::Button(name.c_str(), {ImGui::CalcItemWidth(), lineHeight})) {
            //TODO 跳转对应文件夹
            FileSystemWindow::localPath = path1.parent_path().string();
        }

        if (ImGui::BeginDragDropTarget())
        {
            // TODO 添加类别判断
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH"))
            {
                const char *path_ = ((char *)payload->Data);
                path = path_;
                res = true;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }
};
