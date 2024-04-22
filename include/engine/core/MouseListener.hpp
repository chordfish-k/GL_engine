#pragma once

#include <GLFW/glfw3.h>

#define MOUSE_NUM 3

class MouseListener {
private:
    static MouseListener *instance; // 单例

    double scrollX, scrollY;
    double xPos, yPos, lastX, lastY;
    bool mouseButtonPressed[MOUSE_NUM];
    bool isDragging;

    MouseListener()
        : scrollX(0), scrollY(0), xPos(0), yPos(0), lastX(0), lastY(0) {}

public:
    static MouseListener *Get();

    static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                    int mods);

    static void MousePosCallback(GLFWwindow *window, double xPos, double yPos);

    static void MouseScrollCallback(GLFWwindow *window, double xOffset,
                                    double yOffset);

    static void EndFrame();

    static bool IsMouseButtonDown(int button);

    // 属性
    static float GgetX() { return (float)Get()->xPos; };

    static float GetY() { return (float)Get()->yPos; };

    static float GetScrollX() { return (float)Get()->scrollX; };

    static float GetScrollY() { return (float)Get()->scrollY; };

    static float GetDx() { return (float)(Get()->lastX - Get()->xPos); };

    static float GetDy() { return (float)(Get()->lastY - Get()->yPos); };

    static bool IsDragging() { return Get()->isDragging; }
};
