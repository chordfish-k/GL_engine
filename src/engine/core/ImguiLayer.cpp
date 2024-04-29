﻿#include "engine/core/ImguiLayer.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/SceneHierarchyWindow.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/editor/MenuBar.hpp"
#include <imgui.h>

ImguiLayer::ImguiLayer(GLFWwindow *glfwWindow) : glfwWindow(glfwWindow) {}

void ImguiLayer::InitImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = "imgui.ini";
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendPlatformName = "imgui_impl_glfw";

/*
    // 回调
    glfwSetKeyCallback(glfwWindow, [](auto w, auto key, auto scancode,
                                      auto action, auto mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (action == GLFW_PRESS) {
            io.KeysDown[key] = true;
        } else if (action == GLFW_RELEASE) {
            io.KeysDown[key] = false;
        }

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        if (!io.WantCaptureKeyboard) {
            KeyListener::KeyCallback(w, key, scancode, action, mods);
        }
    });

    glfwSetCharCallback(glfwWindow, [](auto w, auto c) {
        ImGuiIO &io = ImGui::GetIO();
        if (c != GLFW_KEY_DELETE) {
            io.AddInputCharacter(c);
        }
    });

    glfwSetMouseButtonCallback(
        glfwWindow, [](auto w, auto button, auto action, auto mods) {
            ImGuiIO &io = ImGui::GetIO();

            io.MouseDown[0] =
                button == GLFW_MOUSE_BUTTON_1 && action != GLFW_RELEASE;
            io.MouseDown[1] =
                button == GLFW_MOUSE_BUTTON_2 && action != GLFW_RELEASE;
            io.MouseDown[2] =
                button == GLFW_MOUSE_BUTTON_3 && action != GLFW_RELEASE;
            io.MouseDown[3] =
                button == GLFW_MOUSE_BUTTON_4 && action != GLFW_RELEASE;
            io.MouseDown[4] =
                button == GLFW_MOUSE_BUTTON_5 && action != GLFW_RELEASE;


            if (!io.WantCaptureMouse && io.MouseDown[1]) {
                ImGui::SetWindowFocus(nullptr);
            }

            if (!io.WantCaptureMouse || (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)) {
                MouseListener::MouseButtonCallback(w, button, action, mods);
            }
        });

    glfwSetScrollCallback(glfwWindow, [](auto w, auto xOffset, auto yOffset) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheelH = io.MouseWheelH + (float)xOffset;
        io.MouseWheel = io.MouseWheel + (float)yOffset;

        if (!io.WantCaptureMouse) {
            MouseListener::MouseScrollCallback(w, xOffset, yOffset);
        }
    });

    io.SetClipboardTextFn = [](auto user_data, auto text) {
        glfwSetClipboardString((GLFWwindow *)user_data, text);
    };

    io.GetClipboardTextFn = [](auto user_data) {
        return glfwGetClipboardString((GLFWwindow *)user_data);
    };
*/
    // Fonts

    auto fontAtlas = io.Fonts;
    auto fontConfig = ImFontConfig();
    fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesChineseSimplifiedCommon();

    fontConfig.PixelSnapH = true;
    fontAtlas->AddFontFromFileTTF("assets/font/msyh.ttc", 22);

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImguiLayer::Update(float dt) {
    StartFrame(dt);

    SetupDockspace();
    if (Window::CurrentScene() != nullptr)
        Window::CurrentScene()->SceneImgui();
//    ImGui::ShowDemoWindow();
    GameViewWindow::Imgui();
    PropertiesWindow::Imgui();
    SceneHierarchyWindow::Imgui();

    EndFrame();
}

void ImguiLayer::StartFrame(float dt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
//
//
//    // Get window properties and mouse position
//    int winWidth = Window::GetWidth();
//    int winHeight = Window::GetHeight();
//
//    double mousePosX = 0;
//    double mousePosY = 0;
//    glfwGetCursorPos(glfwWindow, &mousePosX, &mousePosY);
//
//    // We SHOULD call those methods to update Dear ImGui state for the current frame
//    ImGuiIO &io = ImGui::GetIO();
//    io.DisplaySize = ImVec2((float)winWidth, (float)winHeight);
//    //.S大化窗口大小可能小于设定的帧缓冲区大小，调整偏移的坐标
//    io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
//    io.MousePos = ImVec2((float) mousePosX, (float) mousePosY);
//    io.DeltaTime = dt;
//
//    // Update the mouse cursor
//    ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
//    glfwSetCursor(glfwWindow, mouseCursors[imguiCursor]);
//    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void ImguiLayer::EndFrame() {


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImguiLayer::DestroyImgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiLayer::SetupDockspace() {
    int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(mainViewport->WorkPos);
    ImGui::SetNextWindowSize(mainViewport->WorkSize);
    ImGui::SetNextWindowViewport(mainViewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus;

    bool p_open = true;
    ImGui::Begin("Dockspace Demo", &p_open, windowFlags);
    ImGui::PopStyleVar(2);

    // Dockspace
    ImGui::DockSpace(ImGui::GetID("Dockspace"));

    MenuBar::Imgui();

    ImGui::End();
}