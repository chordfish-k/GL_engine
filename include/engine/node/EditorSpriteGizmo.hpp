#pragma once

#include "Node.hpp"

COMPONENT(EditorSpriteGizmo)
public:
    enum GizmoMode{
        NONE,
        MOVE,
        ROTATE,
        SCALE
    };
    GizmoMode mode = NONE;
    Node *activeNode = nullptr;
    Node *lastActiveNode = nullptr;

private:
    std::vector<glm::vec2> scaleCircleCenter;
    std::vector<glm::vec2> rotateCircleCenter;
    glm::vec2 spriteCenter;
    float circleR = 4.f;
    int selectedPointIndex = -1;

public:
    void EditorUpdate(float dt) override;

private:
    void DrawBorderAndPoints();

    void CheckAndApplyMove();

    void CheckAndApplyRotation();
    void CheckAndApplyScale();
};
