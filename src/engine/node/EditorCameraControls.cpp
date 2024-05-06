#include "engine/node/EditorCameraControls.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/editor/PropertiesWindow.hpp"

EditorCameraControls::EditorCameraControls(Camera *editorCamera) {
    this->editorCamera = editorCamera;
    this->clickOrigin = {0, 0};
}

void EditorCameraControls::Update(float dt) {
    if (!editorCamera)
        return;

//    if (PropertiesWindow::GetActiveNode())
//        return;

    // 中键拖动
    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && dragDebounce > 0) {
        clickOrigin = {MouseListener::GetWorldX(), MouseListener::GetWorldY()};
        dragDebounce -= dt;
        return;
    } else if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        glm::vec2 mousePos = {MouseListener::GetWorldX(),
                              MouseListener::GetWorldY()};
        glm::vec2 delta = mousePos - clickOrigin;
        auto x = dt * dragSensitivity;
        auto d = delta * glm::vec2(x);
        editorCamera->position = editorCamera->position - d;
        clickOrigin = glm::mix(clickOrigin, mousePos, dt);
    }

    // 防抖
    if (dragDebounce <= 0.f && !MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
        dragDebounce = 0.1f;
    }

    // 滚轮缩放
    if (MouseListener::GetScrollY() != 0.0f) {
        float addValue = (float) pow(abs(MouseListener::GetScrollY() * scrollSensitivity),
                                         1 / editorCamera->GetZoom());
        addValue *= -(float)(MouseListener::GetScrollY() >= 0 ? 1 : -1);
        editorCamera->AddZoom(addValue);
        editorCamera->AdjustProjection();
    }

    // 小数点键：返回原点坐标
    if (KeyListener::IsKeyPressed(GLFW_KEY_KP_DECIMAL)) {
        reset = true;
    }

    if (reset) {
        editorCamera->position = glm::mix(editorCamera->position, {0, 0}, lerpTime);
        editorCamera->SetZoom(editorCamera->GetZoom() +
                                  (1.0f - editorCamera->GetZoom()) * lerpTime);
        lerpTime += 0.1f * dt;

        if (abs(editorCamera->position.x) <= 0.5f &&
            abs(editorCamera->position.y) <= 5.0f) {
            editorCamera->position = {0, 0};
            editorCamera->SetZoom(1.0f);
            reset = false;
        }
        editorCamera->AdjustProjection();
    }
}

void EditorCameraControls::SetEditorCamera(Camera *editorCamera) {
    this->editorCamera = editorCamera;
}
