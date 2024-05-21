#include "engine/core/ImguiLayer.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/SceneHierarchyWindow.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/editor/FileSystemWindow.hpp"
#include "engine/editor/MenuBar.hpp"
#include "engine/editor/FileDialog.hpp"
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
//    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
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
    fontConfig.SizePixels = 18;
//    fontAtlas->AddFontDefault(&fontConfig);
    fontAtlas->AddFontFromFileTTF("assets/font/msyh.ttc", 20, &fontConfig);
//    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImguiLayer::Update(float dt) {
    StartFrame(dt);

    PropertiesWindow::Update(dt);

    ProjectManagerWindow::Imgui();

    SetupDockspace();
    if (Window::CurrentScene() != nullptr)
        Window::CurrentScene()->SceneImgui();

    GameViewWindow::Imgui();
    PropertiesWindow::Imgui();
    SceneHierarchyWindow::Imgui();
    FileSystemWindow::Imgui();

    FileDialog::Imgui();

    EndFrame();
}

void ImguiLayer::StartFrame(float dt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void ImguiLayer::EndFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    auto backupWindowPtr = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupWindowPtr);

}

void ImguiLayer::DestroyImgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiLayer::SetupDockspace() {
    int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetWindowViewport();
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    ImVec2 windowPos = ImVec2(viewport->Pos.x - mainViewport->Pos.x, viewport->Pos.y - mainViewport->Pos.y);

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