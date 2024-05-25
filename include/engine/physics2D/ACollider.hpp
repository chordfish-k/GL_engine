#pragma once

#include <glm/vec2.hpp>

class RigidBody2D;

class ACollider {
protected:
    glm::vec2 offset = {0, 0};
public:
    virtual ~ACollider() = default;

    virtual void EditorUpdate(float dt, RigidBody2D *rb) = 0;

    glm::vec2 GetOffset() {
        return offset;
    }
};
