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

    static bool DrawIntSpinner(const std::string& label, int &values, int v_min=INT32_MIN, int v_max=INT32_MAX);

    static bool DrawIntControl(const std::string& label, int &values, int v_speed = 1, int v_min=0, int v_max=0);

    static bool DrawFloatControl(const std::string& label, float &values);

    static bool DrawCheckbox(const std::string& label, bool &values);

    static bool DrawVec2Control(const std::string& label, glm::vec2 &values, float resetValue = 0.f, float speed = 0.1f, float columnWidth = COLUMN_WIDTH);

    static bool DrawVec3Control(const std::string& label, glm::vec3 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH);

    static bool DrawVec4Control(const std::string& label, glm::vec4 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH);

    static bool DrawColor4Control(const std::string& label, glm::vec4 &color);

    static bool DrawResourceDragDropBox(const std::string &label, std::string &path);

    static bool DrawTextInput(const std::string &label, std::string &value);
};
