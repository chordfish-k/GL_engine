#include "engine/core/Window.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/core/MouseListener.hpp"

#include <GLFW/glfw3.h>
#include "engine/editor/ProjectManagerWindow.hpp"

Window *Window::Get() {
    return new Window();
}

void Window::Loop() {
    while (!glfwWindowShouldClose(this->glfwWindow)) {
        // 轮询事件
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow); // 双缓冲交换
    }
}

void Window::Init() {
    // 设置错误信息回调
    glfwSetErrorCallback(&util::GLFWErrorCallBack);

    // 初始化GLFW
    assert(glfwInit() != 0);

    // 配置GLFW
    glfwDefaultWindowHints(); // 基础默认配置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // 创建一个窗口
    glfwWindow = glfwCreateWindow(this->width, this->height,
                                        this->title.c_str(), nullptr, nullptr);
    // assert(this->glfwWindow != nullptr);
    if (glfwWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    // 鼠标和键盘监听
    glfwSetCursorPosCallback(glfwWindow, &MouseListener::MousePosCallback);
    glfwSetMouseButtonCallback(glfwWindow, &MouseListener::MouseButtonCallback);
    glfwSetScrollCallback(glfwWindow, &MouseListener::MouseScrollCallback);
    glfwSetKeyCallback(glfwWindow, &KeyListener::KeyCallback);
    // 窗口大小变化监听
    glfwSetWindowSizeCallback(
        glfwWindow, [](GLFWwindow *winPtr, int newWidth, int newHeight) {
        });

    // 使opengl上下文成为当前上下文
    glfwMakeContextCurrent(glfwWindow);
    // 启用v-sync垂直同步，帧刷新率尽量高
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    // 使窗口可见
    glfwShowWindow(this->glfwWindow);

    glViewport(0, 0, Setting::GAME_VIEW_BUFFER_W, Setting::GAME_VIEW_BUFFER_H);
}

void Window::Run() {
    util::Println("Hello", "GLFW", "!");

    Get()->Init();
    Get()->Loop();

    // 释放内存
    glfwDestroyWindow(Get()->glfwWindow);

    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

GLFWwindow *Window::GetGlfwWindow(){
    return glfwWindow;
}
