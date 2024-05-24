﻿#pragma once

#include "engine/core/Window.hpp"
#include "engine/event/IObserver.hpp"
#include "engine/event/EventSystem.hpp"

class MainWindow : public Window, public IObserver{
protected:
    static MainWindow *window; // 单例模式
    Scene *currentScene = nullptr;
    ImguiLayer *imguiLayer = nullptr;
    FrameBuffer *frameBuffer = nullptr;
    PickingTexture *pickingTexture = nullptr;

    MainWindow() {
        r = g = b = 0.2f;
        width = Setting::WINDOW_W;
        height=Setting::WINDOW_H;
        title="Window";
        EventSystem::AddObserver(this);
    }

public:
    ~MainWindow() override;

    static MainWindow *Get();

    void Run() override;

    void Init() override;

    void Loop() override;

    static void ChangeScene(ASceneInitializer *sceneInitializer);

    void Notify(Node *node, Event event) override;

    // 属性
    static void SetTitle(const std::string &title) { Get()->title = title; }

    static int GetWidth() { return Get()->width; }

    static int GetHeight() { return Get()->height; }

    static void SetWidth(int width) { Get()->width = width; }

    static void SetHeight(int height) { Get()->height = height; }

    static Scene *GetScene() { return Get()->currentScene; }

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
