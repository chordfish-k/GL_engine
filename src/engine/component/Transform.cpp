#include "engine/component/Transform.hpp"
#include "engine/util/Print.hpp"

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

std::string Transform::Serialize() {
    json j;
    j["component"] = componentName;
    j["position"] = {position.x, position.y};
    j["scale"] = {scale.x, scale.y};
    return j.dump(2);
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
