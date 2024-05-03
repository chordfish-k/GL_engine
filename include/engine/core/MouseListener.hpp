#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#define MOUSE_NUM 9

class MouseListener {
private:
    static MouseListener *instance; // 单例

    double scrollX, scrollY;
    double xPos, yPos, lastX, lastY;
    double worldX, worldY, lastWorldX, lastWorldY;
    bool mouseButtonPressed[MOUSE_NUM];
    bool lastMouseButtonPressed[MOUSE_NUM];
    int mouseButtonDown = 0;
    bool isDragging;

    glm::vec2 gameViewportPos = {0, 0};
    glm::vec2 gameViewportSize = {0, 0};

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

    static float GetScreenX();

    static float GetScreenY();

    static glm::vec2 GetScreenPos();

    static float GetWorldX();

    static float GetWorldY();

    static glm::vec2 GetWorldPos();

    static void SetGameViewportPos(glm::vec2 gameViewportPos) {Get()->gameViewportPos = gameViewportPos;}

    static void SetGameViewportSize(glm::vec2 gameViewportSize) {Get()->gameViewportSize = gameViewportSize;}

    static float GetX() { return (float)Get()->xPos; };

    static float GetY() { return (float)Get()->yPos; };

    static float GetScrollX() { return (float)Get()->scrollX; };

    static float GetScrollY() { return (float)Get()->scrollY; };

    static float GetDx() { return (float)(Get()->lastX - Get()->xPos); };

    static float GetDy() { return (float)(Get()->lastY - Get()->yPos); };

    static bool IsDragging() { return Get()->isDragging; }
};
