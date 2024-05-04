#include "engine/node/EditorCameraControls.hpp"
#include "engine/core/MouseListener.hpp"

EditorCameraControls::EditorCameraControls(Camera *editorCamera) {
    this->editorCamera = editorCamera;
    this->clickOrigin = {0, 0};
}

void EditorCameraControls::Update(float dt) {
    if (!editorCamera)
        return;

    // 中键拖动
    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && dragDebounce > 0) {
        clickOrigin = {MouseListener::GetWorldX(), MouseListener::GetWorldY()};
        dragDebounce -= dt;
        return;
    } else if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        glm::vec2 mousePos = {MouseListener::GetWorldX(), MouseListener::GetWorldY()};
        glm::vec2 delta = mousePos - clickOrigin;
        auto x = dt * dragSensitivity;
        auto d = delta * glm::vec2(x);
        editorCamera->position = editorCamera->position - d;
        clickOrigin = glm::mix(clickOrigin, mousePos, dt);
    }

    if (dragDebounce <= 0.f && !MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        dragDebounce = 0.1f;
    }
}

void EditorCameraControls::SetEditorCamera(Camera *editorCamera) {
    this->editorCamera = editorCamera;
}
