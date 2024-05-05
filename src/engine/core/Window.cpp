#include "engine/core/Window.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/TestScene.hpp"

#include <GLFW/glfw3.h>
#include "engine/reflect/Reflect.hpp"


Window *Window::window = nullptr;

AScene *Window::currentScene = nullptr;

Window::~Window() {
    AssetPool::Clear();
    delete imguiLayer;
    delete frameBuffer;
    delete pickingTexture;
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

    Shader *defaultShader = AssetPool::GetShader("assets/shader/default.glsl");
    Shader *pickingShader = AssetPool::GetShader("assets/shader/pickingShader.glsl");

    while (!glfwWindowShouldClose(this->glfwWindow)) {
        // 轮询事件
        glfwPollEvents();

        // 渲染到拾取纹理
        glDisable(GL_BLEND);
        pickingTexture->EnableWriting();

        glViewport(0, 0, 3840, 2160);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::BindShader(pickingShader);
        currentScene->Render();

//        if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
//            int x = (int) MouseListener::GetScreenX();
//            int y = (int) MouseListener::GetScreenY();
//            util::Println(pickingTexture->ReadPixel(x, y)-1);
//        }

        pickingTexture->DisableWriting();
        glEnable(GL_BLEND);


        frameBuffer->Bind();

        // 设置清屏颜色
        glViewport(0, 0, 3840, 2160);
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DebugDraw::BeginFrame();

        // 更新当前场景
        Renderer::BindShader(defaultShader);
        if (dt >= 0) {
            currentScene->Update(dt);
            DebugDraw::Draw();
        }

        frameBuffer->Unbind();

        // 更新gui
        imguiLayer->Update(dt);

        glfwSwapBuffers(glfwWindow); // 双缓冲交换

        MouseListener::EndFrame();

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
    glfwWindow = glfwCreateWindow(this->width, this->height,
                                        this->title.c_str(), nullptr, nullptr);
    // assert(this->glfwWindow != nullptr);
    if (window == nullptr) {
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // 创建帧缓冲
    frameBuffer = new FrameBuffer(3840, 2160);
    // 创建拾取纹理
    pickingTexture = new PickingTexture(3840, 2160);
    glViewport(0, 0, 3840, 2160);

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