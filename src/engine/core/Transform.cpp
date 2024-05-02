#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "engine/core/Transform.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale, float rotation)
    :position(position), scale(scale), rotation(rotation){}


Transform Transform::Copy() const {
    return Transform(position, scale, rotation);
}

void Transform::CopyTo(Transform &to) const {
    to.position = position;
    to.scale = scale;
    to.rotation = rotation;
}

Transform &Transform::operator=(const Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    this->rotation = t.rotation;
    return *this;
}

bool Transform::Equals(Transform &t) {
    return t.position == this->position &&
           t.scale == this->scale &&
           t.rotation == this->rotation;
}

