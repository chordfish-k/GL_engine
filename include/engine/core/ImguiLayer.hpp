#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct GLFWwindow;

class ImguiLayer {
private:
    GLFWwindow *glfwWindow;
    GLFWcursor *mouseCursors[ImGuiMouseCursor_COUNT];

public:
    ImguiLayer(GLFWwindow *glfwWindow);

    void InitImgui();

    void Update(float dt);

    void DestroyImgui();

private:
    void StartFrame(float dt);

    void EndFrame();

    void SetupDockspace();
};