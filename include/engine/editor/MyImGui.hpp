#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <imgui.h>
#include <glm/vec4.hpp>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include "engine/util/Print.hpp"
#include "engine/editor/FileSystemWindow.hpp"

#define COLUMN_WIDTH 100

class MyImGui {
public:

    static bool DrawIntSpinner(const std::string& label, int &values, int v_min=INT32_MIN, int v_max=INT32_MAX);

    static bool DrawIntControl(const std::string& label, int &values, int v_speed = 1, int v_min=INT32_MIN, int v_max=INT32_MAX);

    static bool DrawFloatControl(const std::string& label, float &values, float v_speed = 0.1f, float v_min=std::numeric_limits<float>::lowest(), float v_max=std::numeric_limits<float>::max());

    static bool DrawCheckbox(const std::string& label, bool &values);

    static bool DrawVec2Control(const std::string& label, glm::vec2 &values, float resetValue = 0.f, float speed = 0.1f, float columnWidth = COLUMN_WIDTH);

    static bool DrawVec3Control(const std::string& label, glm::vec3 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH);

    static bool DrawVec4Control(const std::string& label, glm::vec4 &values, float resetValue = 0, float speed = 0.5f, float columnWidth = COLUMN_WIDTH);

    static bool DrawColor4Control(const std::string& label, glm::vec4 &color);

    static bool DrawResourceDragDropBox(const std::string &label, std::string &path, const std::string &type="FILE_PATH");

    static bool DrawResourceDragDropBoxWithBtn(const std::string &label, std::string &path, const std::string &btnTex);

    static bool DrawTextInput(const std::string &label, std::string &value);

    static void DrawText(const std::string &label, std::string &value);

    static bool DrawButton(const std::string &label, std::string &value);

    static bool DrawComboControl(const std::string &label, int &index,
                          const char *values[], int size);
};
