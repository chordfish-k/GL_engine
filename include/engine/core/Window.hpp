#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AScene.hpp"
#include "ImguiLayer.hpp"
#include "engine/core/AScene.hpp"
#include "engine/renderer/FrameBuffer.hpp"
#include <cassert>
#include <string>

class Window {
public:
    float r, g, b;
    bool fadeToBlack;

private:
    static Window *window; // 单例模式
    static AScene *currentScene;
    ImguiLayer *imguiLayer = nullptr;
    FrameBuffer *frameBuffer = nullptr;

    int width;
    int height;
    std::string title;
    GLFWwindow *glfwWindow; // 窗口句柄

    Window() : width(3840), height(2160), title("Window") {
        r = g = b = 0.2f;
        fadeToBlack = false;
    }

public:
    ~Window();

    static Window *Get();

    void Run();

    void Init();

    void Loop();

    static void ChangeScene(int newScene);

    // 属性
    static int GetWidth() { return Get()->width; }

    static int GetHeight() { return Get()->height; }

    static void SetWidth(int width) { Get()->width = width; }

    static void SetHeight(int height) { Get()->height = height; }

    static AScene *GetScene() { return Get()->currentScene; }

    static AScene *CurrentScene() { return Get()->currentScene; }

    static FrameBuffer *GetFrameBuffer() {
        return Get()->frameBuffer;
    }

    static float GetTargetAspectRatio() {
        return 16.0f / 9.0f;
    }
};
