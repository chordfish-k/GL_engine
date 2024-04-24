#include "engine/component/Transform.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale) {
    this->position = position;
    this->scale = scale;
}

void Transform::Init() {}

void Transform::Start() {}

void Transform::Update(float dt) {}

