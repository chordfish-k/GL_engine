#include "engine/physics2D/Box2DCollider.hpp"

const glm::vec2 &Box2DCollider::GetHalfSize() const {
    return size;
}

void Box2DCollider::SetHalfSize(const glm::vec2 &halfSize_) {
    Box2DCollider::size = halfSize_;
}

const glm::vec2 &Box2DCollider::GetOrigin() const {
    return origin;
}

void Box2DCollider::SetOrigin(const glm::vec2 &origin_) {
    Box2DCollider::origin = origin_;
}

void Box2DCollider::EditorUpdate(float dt, RigidBody2D *rb) {
    glm::vec2 center = rb->GetTransform().position + this->offset;
    DebugDraw::AddBox2D(center, size, rb->transform.rotation);
}