#include "engine/core/MouseListener.hpp"

MouseListener *MouseListener::instance = nullptr;

MouseListener *MouseListener::Get() {
    if (instance == nullptr) {
        instance = new MouseListener();
    }
    return instance;
}

void MouseListener::MouseButtonCallback(GLFWwindow *window, int button,
                                        int action, int mods) {

    if (action == GLFW_PRESS) {
        if (button < MOUSE_NUM) {
            Get()->mouseButtonPressed[button] = true;
        }
    } else if (action == GLFW_RELEASE) {
        if (button < MOUSE_NUM) {
            Get()->mouseButtonPressed[button] = false;
            Get()->isDragging = false;
        }
    }
}

void MouseListener::MousePosCallback(GLFWwindow *window, double xPos,
                                     double yPos) {
    Get()->lastX = Get()->xPos;
    Get()->lastY = Get()->yPos;
    Get()->xPos = xPos;
    Get()->yPos = yPos;
    Get()->isDragging = Get()->mouseButtonPressed[0] ||
                        Get()->mouseButtonPressed[1] ||
                        Get()->mouseButtonPressed[2];
}

void MouseListener::MouseScrollCallback(GLFWwindow *window, double xOffset,
                                        double yOffset) {
    Get()->scrollX = xOffset;
    Get()->scrollY = yOffset;
}

void MouseListener::EndFrame() {
    Get()->scrollX = 0;
    Get()->scrollY = 0;
    Get()->lastX = Get()->xPos;
    Get()->lastY = Get()->yPos;
}

bool MouseListener::IsMouseButtonDown(int button) {
    if (button < MOUSE_NUM) {
        return Get()->mouseButtonPressed[button];
    } else {
        return false;
    }
}