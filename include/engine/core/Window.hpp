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
protected:
//    static Window *window; // 单例模式

    int width;
    int height;
    std::string title;
    GLFWwindow *glfwWindow; // 窗口句柄
    float r, g, b;

    Window()
        : width(Setting::WINDOW_W), height(Setting::WINDOW_H),title("Window") {
        r = g = b = 0.2f;
    }

public:
    virtual ~Window() = default;

    static Window *Get(); // 工厂模式

    virtual void Run();

    virtual void Init();

    virtual void Loop();


    // 属性
    void SetTitle(const std::string &title) { Get()->title = title; }

    int GetWidth() { return Get()->width; }
    int GetHeight() { return Get()->height; }
    void SetWidth(int width) { Get()->width = width; }
    void SetHeight(int height) { Get()->height = height; }

    GLFWwindow *GetGlfwWindow();
};
