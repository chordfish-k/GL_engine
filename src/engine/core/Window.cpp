
#include "engine/core/Window.hpp"
#include "engine/core/AScene.hpp"
#include "engine/core/ImguiLayer.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/TestScene.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"
#include <GLFW/glfw3.h>

Window *Window::window = nullptr;

AScene *Window::currentScene = nullptr;

Window::~Window() {
    AssetPool::Clear();
}

Window *Window::Get() {
    if (window == nullptr) {
        window = new Window();
    }
    return window;
}

void Window::Loop() {
    float beginTime = (float)glfwGetTime();
    float endTime;
    float dt = -1.f;

    while (!glfwWindowShouldClose(this->glfwWindow)) {
        // 轮询事件
        glfwPollEvents();

        // 设置清屏颜色
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 更新当前场景
        if (dt >= 0)
            currentScene->Update(dt);

        // 更新gui
        imguiLayer->Update(dt);

        glfwSwapBuffers(glfwWindow); // 双缓冲交换

        // 计算dt
        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;
    }

    currentScene->Save();
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
    this->glfwWindow = glfwCreateWindow(this->width, this->height,
                                        this->title.c_str(), nullptr, nullptr);
    // assert(this->glfwWindow != nullptr);
    if (window == NULL) {
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
            Window::SetWidth(newWidth);
            Window::SetHeight(newHeight);
        });

    // 使opengl上下文成为当前上下文
    glfwMakeContextCurrent(this->glfwWindow);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // 初始化gui
    imguiLayer = new ImguiLayer(glfwWindow);
    imguiLayer->InitImgui();

    // 默认场景
    Window::ChangeScene(0);
}

void Window::Run() {
    util::Println("Hello", "GLFW", "!");

    Get()->Init();
    Get()->Loop();

    // 释放内存
    glfwDestroyWindow(Get()->glfwWindow);

    // 销毁gui
    Get()->imguiLayer->DestroyImgui();
    delete Get()->imguiLayer;

    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

void Window::ChangeScene(int newScene) {
    switch (newScene) {
    case 0:
        currentScene = new TestScene();
        break;
    default:
        util::Print("Unknow scene ", newScene);
        assert(false);
        break;
    }
    currentScene->Load();
    currentScene->Init();
    currentScene->Start();
}