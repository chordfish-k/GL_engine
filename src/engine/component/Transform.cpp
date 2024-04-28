#include "engine/component/Transform.hpp"
#include "engine/util/Print.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale) {
    this->position = position;
    this->scale = scale;
}

void Transform::Init() {}

void Transform::Start() {}

void Transform::Update(float dt) {}

Transform *Transform::Copy() const {
    return new Transform(glm::vec2(position), glm::vec2(scale));
}

void Transform::CopyTo(Transform *to) const {
    to->position = position;
    to->scale = scale;
}

Transform &Transform::operator=(const Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    return *this;
}

bool Transform::Equals(Transform &t) {
    return t.position == this->position && t.scale == this->scale;
}

json Transform::Serialize() {
    json j;
    j["component"] = GetComponentName();
    j["position"] = {position.x, position.y};
    j["scale"] = {scale.x, scale.y};
    return j;
}

Transform *Transform::Deserialize(json j) {
    auto p = j["position"];
    position.x = p[0];
    position.y = p[1];

    auto s = j["scale"];
    scale.x = s[0];
    scale.y = s[1];

    return this;
}

void Transform::Imgui() {
    Component::Imgui<Transform>();
}
