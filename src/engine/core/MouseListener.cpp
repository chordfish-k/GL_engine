#include <glm/vec4.hpp>
#include "engine/core/MouseListener.hpp"
#include "engine/core/Window.hpp"
#include "engine/core/Camera.hpp"
#include "engine/util/Print.hpp"

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
        Get()->mouseButtonDown++;

        if (button < MOUSE_NUM) {
            Get()->mouseButtonPressed[button] = true;
        }
    } else if (action == GLFW_RELEASE) {
        Get()->mouseButtonDown--;

        if (button < MOUSE_NUM) {
            Get()->mouseButtonPressed[button] = false;
            Get()->isDragging = false;
        }
    }
}

void MouseListener::MousePosCallback(GLFWwindow *window, double xPos,
                                     double yPos) {
    if (Get()->mouseButtonDown > 0) {
        Get()->isDragging = true;
    }
    Get()->lastX = Get()->xPos;
    Get()->lastY = Get()->yPos;
    Get()->lastWorldX = Get()->worldX;
    Get()->lastWorldY = Get()->worldY;
    Get()->xPos = xPos + ImGui::GetMainViewport()->Pos.x;
    Get()->yPos = yPos + ImGui::GetMainViewport()->Pos.y;
    GetWorldPos();

//    Get()->isDragging = Get()->mouseButtonPressed[0] ||
//                        Get()->mouseButtonPressed[1] ||
//                        Get()->mouseButtonPressed[2];
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
    Get()->lastWorldX = Get()->worldX;
    Get()->lastWorldY = Get()->worldY;
    for (int i = 0; i < 9; i++) {
        Get()->lastMouseButtonPressed[i] = Get()->mouseButtonPressed[i];
    }
}

bool MouseListener::IsMouseButtonDown(int button) {
    if (button < MOUSE_NUM) {
        return Get()->mouseButtonPressed[button];
    } else {
        return false;
    }
}

bool MouseListener::IsMouseButtonPressed(int button) {
    if (button < MOUSE_NUM) {
        return !Get()->lastMouseButtonPressed[button] && Get()->mouseButtonPressed[button];
    } else {
        return false;
    }
}

float MouseListener::GetScreenX() {
    return GetScreenPos().x;
}

float MouseListener::GetScreenY() {
    return GetScreenPos().y;
}

glm::vec2 MouseListener::GetScreenPos() {
    float currentX = GetX() - Get()->gameViewportPos.x;
    currentX = (currentX / Get()->gameViewportSize.x) * 3840;
    float currentY = GetY() - Get()->gameViewportPos.y;
    currentY = 2160-((currentY / Get()->gameViewportSize.y) * 2160);
    return {currentX, currentY};
}

float MouseListener::GetWorldX() {
    return (float) Get()->worldX;
}

float MouseListener::GetWorldY() {
    return (float) Get()->worldY;
}

float MouseListener::GetWorldDx() {
    return (float)(Get()->worldX - Get()->lastWorldX);
}

float MouseListener::GetWorldDy() {
    return (float)(Get()->worldY - Get()->lastWorldY);
}

glm::vec2 MouseListener::GetWorldPos(){
    auto camera = Window::GetScene()->GetCamera();

    float currentX = GetX() - Get()->gameViewportPos.x;
    currentX = (currentX / Get()->gameViewportSize.x) * 2.f - 1.f;
    float currentY = GetY() - Get()->gameViewportPos.y;
    currentY = -((currentY / Get()->gameViewportSize.y) * 2.f - 1.f);

    glm::vec4 tmp = {currentX, currentY, 0, 1};

    auto invView = camera->GetInvView();
    auto invProj = camera->GetInvProjection();
    tmp = tmp * invView * invProj;

    Get()->worldX = tmp.x  + camera->position.x;
    Get()->worldY = tmp.y  + camera->position.y;
    return tmp;
}
