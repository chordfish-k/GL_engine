#include "engine/physics2D/ACollider.hpp"

ACollider::~ACollider() = default;

void ACollider::EditorUpdate(float dt) {  };

glm::vec2 ACollider::GetOffset() {
    return offset;
}

b2Fixture *ACollider::GetFixture() const {
    return fixture;
}

void ACollider::SetFixture(b2Fixture *fixture_) {
    fixture = fixture_;
}
