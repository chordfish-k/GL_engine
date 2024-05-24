#include "engine/core/ManagerWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/KeyListener.hpp"

ManagerWindow::ManagerWindow() {
    width = 300;
    height = 160;
}

void ManagerWindow::Run() {
    if (!glfwInit())
        return;

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui + GLFW3 Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // 初始化OpenGL加载器
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
    }

    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘导航
    // 设置Dear ImGui样式（可选）
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // 初始化Dear ImGui的渲染器后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入事件
        glfwPollEvents();

        // 开始新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 渲染Dear ImGui的UI
        {
            const float buttonWidth = ImGui::GetContentRegionAvail().x * 0.5f; // 获取当前窗口内容的宽度并平分
            if (ImGui::Begin("My Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                // 第一个按钮
                if (ImGui::Button("Button 1", ImVec2(buttonWidth, 0.0f)))
                {
                    // 处理按钮点击事件
                    printf("Button 1 clicked\n");
                }
                ImGui::SameLine(); // 下一项在同一行开始

                // 第二个按钮
                if (ImGui::Button("Button 2", ImVec2(buttonWidth, 0.0f)))
                {
                    // 处理按钮点击事件
                    printf("Button 2 clicked\n");
                }
            }
            ImGui::End();
        }

        // 渲染Dear ImGui的绘制列表
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 交换缓冲区
        glfwSwapBuffers(window);
    }

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
void ManagerWindow::Init() {
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
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);


    // 创建一个窗口
    glfwWindow = glfwCreateWindow(this->width, this->height,
                                  this->title.c_str(), nullptr, nullptr);

    // assert(this->glfwWindow != nullptr);
    if (glfwWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        glfwSetWindowAttrib(glfwWindow, GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    }

    // 鼠标和键盘监听
    glfwSetCursorPosCallback(glfwWindow, &MouseListener::MousePosCallback);
    glfwSetMouseButtonCallback(glfwWindow, &MouseListener::MouseButtonCallback);
    glfwSetScrollCallback(glfwWindow, &MouseListener::MouseScrollCallback);
    glfwSetKeyCallback(glfwWindow, &KeyListener::KeyCallback);

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
}
void ManagerWindow::Loop() {
    while (!glfwWindowShouldClose(this->glfwWindow)) {
        // 轮询事件
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //
//        int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
//
//        ImGuiViewport* viewport = ImGui::GetWindowViewport();
//        ImGuiViewport *mainViewport = ImGui::GetMainViewport();
//        ImVec2 windowPos = ImVec2(viewport->Pos.x - mainViewport->Pos.x, viewport->Pos.y - mainViewport->Pos.y);
//
//        ImGui::SetNextWindowPos(mainViewport->WorkPos);
//        ImGui::SetNextWindowSize(mainViewport->WorkSize);
//        ImGui::SetNextWindowViewport(mainViewport->ID);
//
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
//        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {0, 0});
//        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
//                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
//                       ImGuiWindowFlags_NoBringToFrontOnFocus |
//                       ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
//
//        bool p_open = true;
//        ImGui::Begin("Dockspace Demo", &p_open, windowFlags);
//        ImGui::PopStyleVar(5);
//
//        // Dockspace
//        ImGui::DockSpace(ImGui::GetID("Dockspace"));
//
//        ImGui::End();

//        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
//        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {0, 0});
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//        ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
//        ImGui::Button("A", ImGui::GetContentRegionAvail());
//        ImGui::PopStyleVar(5);
//        ImGui::End();

        ImGui::ShowDemoWindow();

        //

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glViewport(0, 0, MainWindow::GetWidth(), MainWindow::GetHeight());
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        auto backupWindowPtr = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupWindowPtr);

        glfwSwapBuffers(glfwWindow); // 双缓冲交换
    }
}
