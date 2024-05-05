#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AScene.hpp"
#include "ImguiLayer.hpp"
#include "engine/core/AScene.hpp"
#include "engine/renderer/FrameBuffer.hpp"
#include "engine/renderer/PickingTexture.hpp"
#include <cassert>
#include <string>

class Window {
public:
    float r, g, b;

private:
    static Window *window; // 单例模式
    static AScene *currentScene;
    ImguiLayer *imguiLayer = nullptr;
    FrameBuffer *frameBuffer = nullptr;
    PickingTexture *pickingTexture = nullptr;

    int width;
    int height;
    const int buffWidth;
    const int buffHeight;
    std::string title;
    GLFWwindow *glfwWindow; // 窗口句柄

    Window()
        : width(1920), height(1080),
          buffWidth(width<<1), buffHeight(height<<1), title("Window") {
        r = g = b = 0.2f;
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

    static PickingTexture *GetPickingTexture() {
        return Get()->pickingTexture;
    }

    static float GetTargetAspectRatio() {
        return 16.0f / 9.0f;
    }
};
