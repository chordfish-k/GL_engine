#include "engine/physics2D/CircleCollider.hpp"

float CircleCollider::GetRadius() const {
    return radius;
}

void CircleCollider::SetRadius(float radius_) {
    CircleCollider::radius = radius_;
}

void CircleCollider::EditorUpdate(float dt, RigidBody2D *rb) {}
