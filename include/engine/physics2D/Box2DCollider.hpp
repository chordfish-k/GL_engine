#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/renderer/DebugDraw.hpp"

class Box2DCollider : public ACollider {
private:
    glm::vec2 size = {1, 1};
    glm::vec2 origin = {0, 0};

public:
    const glm::vec2 &GetHalfSize() const;

    void SetHalfSize(const glm::vec2 &halfSize);

    const glm::vec2 &GetOrigin() const;

    void SetOrigin(const glm::vec2 &origin);

    void EditorUpdate(float dt, RigidBody2D *rb);
};
