#pragma once

#include <GLFW/glfw3.h>

#define KEY_NUM 350

class KeyListener {
private:
    static KeyListener *instance; // 单例

    bool keyPressed[KEY_NUM] = {false};
    bool lastKeyPressed[KEY_NUM] = {false};

    KeyListener() {}

public:
    static KeyListener *Get();

    static void KeyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods);

    static void EndFrame();

    static bool IsKeyDown(int keyCode);

    static bool IsKeyPressed(int keyCode);

    static void DoShortcutKeys();
};
