#include "engine/core/MouseListener.hpp"
#include "engine/core/Window.hpp"

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

float MouseListener::GetOrthoX() {
    float currentX = GetX();
    currentX = (currentX / (float) Window::GetWidth()) * 2.f - 1.f;
    glm::vec4 tmp = {currentX, 0, 0, 1};
    tmp = tmp * Window::GetScene()->GetCamera()->GetInvProjection();
    currentX = tmp.x;
    util::Println("currentX: ", currentX);
    return currentX;
}

float MouseListener::GetOrthoY() {
    float currentY = GetY();
    currentY = (currentY / (float) Window::GetHeight()) * 2.f - 1.f;
    glm::vec4 tmp = {currentY, 0, 0, 1};
    tmp = tmp * Window::GetScene()->GetCamera()->GetInvProjection();
    currentY = tmp.y;
    util::Println("currentY: ", currentY);
    return currentY;
}

