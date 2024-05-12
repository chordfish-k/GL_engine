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

private:
    std::vector<glm::vec2> scaleCircleCenter;
    std::vector<glm::vec2> rotateCircleCenter;
    glm::vec2 spriteCenter;
    float circleR = 4.f;

public:
    void Update(float dt) override;

private:
    void DrawBorderAndPoints();

    void CheckAndApplyMove();

    void CheckAndApplyRotation();
};
