#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.hpp"
#include "ImguiLayer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/renderer/FrameBuffer.hpp"
#include "engine/renderer/PickingTexture.hpp"
#include "engine/util/Setting.hpp"
#include <cassert>
#include <string>

class Window {
public:
    float r, g, b;

private:
    static Window *window; // 单例模式
    Scene *currentScene = nullptr;
    ImguiLayer *imguiLayer = nullptr;
    FrameBuffer *frameBuffer = nullptr;
    PickingTexture *pickingTexture = nullptr;

    bool showingProjectManager = true;

    int width;
    int height;
    std::string title;
    GLFWwindow *glfwWindow; // 窗口句柄

    Window()
        : width(Setting::WINDOW_W), height(Setting::WINDOW_H),title("Window") {
        r = g = b = 0.2f;
    }

public:
    ~Window();

    static Window *Get();

    void Run();

    void Init();

    void Loop();

    static void ChangeScene(ASceneInitializer *sceneInitializer);

    // 属性
    static void SetTitle(const std::string &title) { Get()->title = title; }

    static int GetWidth() { return Get()->width; }

    static int GetHeight() { return Get()->height; }

    static void SetWidth(int width) { Get()->width = width; }

    static void SetHeight(int height) { Get()->height = height; }

    static Scene *GetScene() { return Get()->currentScene; }

    static Scene *CurrentScene() { return Get()->currentScene; }

    static bool IsShowingProjectManagerWindow() {return Get()->showingProjectManager;}

    static void SetShowingProjectManagerWindow(bool show) {Get()->showingProjectManager = show;}

    static FrameBuffer *GetFrameBuffer() {
        return Get()->frameBuffer;
    }

    static PickingTexture *GetPickingTexture() {
        return Get()->pickingTexture;
    }

    static float GetTargetAspectRatio() {
        return Setting::GAME_VIEW_ASPECT;
    }

    static GLFWwindow *GetGlfwWindow();
};
