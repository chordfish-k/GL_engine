#include "engine/component/Transform.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale) {
    this->position = position;
    this->scale = scale;
}

void Transform::Init() {}

void Transform::Start() {}

void Transform::Update(float dt) {}

Transform *Transform::Copy() {
    return new Transform(glm::vec2(position), glm::vec2(scale));
}

void Transform::CopyTo(Transform *to) {
    to->position = position;
    to->scale = scale;
}

Transform &Transform::operator=(Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    return *this;
}

bool Transform::Equals(Transform &t) {
    return t.position == this->position && t.scale == this->scale;
}
