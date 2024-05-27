#include "engine/core/MainWindow.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/EditorSceneInitializer.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/editor/ProjectManagerWindow.hpp"

#include "engine/reflect/Reflect.hpp"

MainWindow *MainWindow::window = nullptr;

MainWindow::~MainWindow() {
    AssetPool::Clear();

}

MainWindow *MainWindow::Get() {
    if (window == nullptr) {
        window = new MainWindow();
    }
    return window;
}

void MainWindow::Loop() {
    float beginTime = (float)glfwGetTime();
    float endTime;
    float dt = -1.f;

    Shader *defaultShader = AssetPool::GetShader("assets/shader/default.glsl");
    Shader *pickingShader = AssetPool::GetShader("assets/shader/pickingShader.glsl");

    while (!glfwWindowShouldClose(this->glfwWindow)) {
        // 轮询事件
        glfwPollEvents();

        // 更新事件
        // 快捷键事件
        KeyListener::DoShortcutKeys();

//        auto currentScene = Get()->currentScene;

        if (currentScene) {
            // 渲染到拾取纹理
            glDisable(GL_BLEND);
            pickingTexture->EnableWriting();

            glViewport(0, 0, 3840, 2160);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Renderer::BindShader(pickingShader);
            currentScene->Render();

            pickingTexture->DisableWriting();
            glEnable(GL_BLEND);

            frameBuffer->Bind();

            // 设置清屏颜色
            glViewport(0, 0, Setting::GAME_VIEW_BUFFER_W, Setting::GAME_VIEW_BUFFER_H);
            glClearColor(r, g, b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 更新当前场景
            Renderer::BindShader(defaultShader);
            if (dt >= 0) {
                if (!runtimePlaying) {
                    currentScene->EditorUpdate(dt);
                    currentScene->Render();

                    DebugDraw::BeginFrame();
                    DebugDraw::Draw();
                } else {
                    currentScene->Update(dt);
                    currentScene->Render();

                    DebugDraw::BeginFrame();
                    DebugDraw::Draw();

                }
            }

            frameBuffer->Unbind();
        }


        // 更新gui
        imguiLayer->EditorUpdate(dt);

        glfwSwapBuffers(glfwWindow); // 双缓冲交换

        MouseListener::EndFrame();
        KeyListener::EndFrame();

        // 计算dt
        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;

        if (currentScene && currentScene->shouldDestroy) {
            delete currentScene;
            Get()->currentScene = new Scene(nextScene);
            Get()->currentScene->Load();
            Get()->currentScene->Init();
            Get()->currentScene->Start();
        }
    }
}

void MainWindow::Init() {
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
            MainWindow::SetWidth(newWidth);
            MainWindow::SetHeight(newHeight);
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
    frameBuffer = new FrameBuffer(Setting::GAME_VIEW_BUFFER_W, Setting::GAME_VIEW_BUFFER_H);
    // 创建拾取纹理
    pickingTexture = new PickingTexture(Setting::GAME_VIEW_BUFFER_W, Setting::GAME_VIEW_BUFFER_H);
    glViewport(0, 0, Setting::GAME_VIEW_BUFFER_W, Setting::GAME_VIEW_BUFFER_H);

    // 初始化gui
    imguiLayer = new ImguiLayer(glfwWindow);
    imguiLayer->InitImgui();

    // 默认场景
    MainWindow::ChangeScene(new EditorSceneInitializer(defaultScenePath));
}

void MainWindow::Run() {
    util::Println("Hello", "GLFW", "!");

    Setting::Load();

    Get()->Init();
    Get()->Loop();

    // 释放内存
    glfwDestroyWindow(Get()->glfwWindow);

    // 销毁gui
    Get()->imguiLayer->DestroyImgui();
    delete Get()->imguiLayer;

    delete Get()->frameBuffer;
    delete Get()->pickingTexture;

    glfwTerminate();
    glfwSetErrorCallback(nullptr);

    Setting::Save();
}

void MainWindow::ChangeScene(ASceneInitializer *sceneInitializer) {
    if (Get()->currentScene != nullptr) {
        // 销毁当前场景
        Get()->currentScene->Destroy();
    }
    PropertiesWindow::SetActiveNode(nullptr);
    if (Get()->currentScene != nullptr) {
        ChangeSceneLazy(sceneInitializer);
    } else {
        Get()->currentScene = new Scene(sceneInitializer);
        Get()->currentScene->Load();
        Get()->currentScene->Init();
        Get()->currentScene->Start();
    }
}

GLFWwindow *MainWindow::GetGlfwWindow(){
    return Get()->glfwWindow;
}

void MainWindow::Notify(Node *node, Event event) {
    switch (event.type) {

    case SaveScene:
        if (currentScene) currentScene->Save();
        break;

    case CloseProject:
        if (currentScene) {
            ChangeScene(new EditorSceneInitializer());
            Setting::PROJECT_ROOT = "";
        }
        break;

    case LoadScene:
        ProjectManagerWindow::shouldOpen = true;
        break;

    case CloseTopWindow:
        if (ProjectManagerWindow::shouldOpen)
            ProjectManagerWindow::shouldOpen = false;
        break;

    case GameEngineStartPlay:
        if (currentScene) {
            currentScene->Save();
            ChangeScene(new EditorSceneInitializer(
                currentScene->GetSceneInitializer()->GetFilePath()));
            runtimePlaying = true;
        }
        break;

    case GameEngineStopPlay:
        if (currentScene) {
            runtimePlaying = false;
            ChangeScene(new EditorSceneInitializer(
                currentScene->GetSceneInitializer()->GetFilePath()));
        }
        break;
    }
}

void MainWindow::ChangeSceneLazy(ASceneInitializer *nextScene_) {
    Get()->nextScene = nextScene_;
}

bool MainWindow::IsPlaying() {
    return Get()->runtimePlaying;
}
