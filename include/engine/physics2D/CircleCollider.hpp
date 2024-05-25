#pragma once

#include "engine/physics2D/ACollider.hpp"

class CircleCollider : public ACollider{
private:
    float radius = 1.f;

public:
    float GetRadius() const;

    void SetRadius(float radius);

    void EditorUpdate(float dt, RigidBody2D *rb) override;
};
