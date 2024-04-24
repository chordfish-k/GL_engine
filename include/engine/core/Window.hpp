#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AbstractScene.hpp"
#include "engine/core/AbstractScene.hpp"
#include <cassert>
#include <string>

class Window {
public:
    float r, g, b;
    bool fadeToBlack;

private:
    static Window *window; // 单例模式

    int width;
    int height;
    std::string title;
    GLFWwindow *glfwWindow; // 窗口句柄
    static AbstractScene *currentScene;

    Window() : width(1920), height(1080), title("Window") {
        r = g = b = 0.2f;
        fadeToBlack = false;
    }

public:
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

    static AbstractScene *GetScene() {return Get()->currentScene; }
};
