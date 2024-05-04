#pragma once

#include "engine/node/Node.hpp"
#include "engine/core/Camera.hpp"

COMPONENT(EditorCameraControls)
private:
    Camera *editorCamera = nullptr;
    glm::vec2 clickOrigin{};
    float lerpTime = 0.0f;
    float dragDebounce = 0.032f;
    float dragSensitivity = 30.0f;
    float scrollSensitivity = 0.1f;
    bool reset = false;

public:
    EditorCameraControls() {};

    EditorCameraControls(Camera *editorCamera);

    void SetEditorCamera(Camera *editorCamera);

    void Update(float dt) override;
};
