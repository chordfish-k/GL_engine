#include "engine/physics2D/ACollider.hpp"

ACollider::~ACollider() = default;

void ACollider::EditorUpdate(float dt) {  };

glm::vec2 ACollider::GetOffset() {
    return offset;
}

std::vector<b2Fixture*> &ACollider::GetFixture() {
    return fixture;
}

void ACollider::SetFixture(b2Fixture *fixture_, int index) {
    if (index >= fixture.size()) return;
    if (fixture[index]) {
        fixture[index]->GetBody()->DestroyFixture(fixture[index]);
    }
    fixture[index] = fixture_;
}

bool ACollider::IsDirty() {
    return isDirty;
}

void ACollider::SetDirty(bool dirty) {
    isDirty = dirty;
}
