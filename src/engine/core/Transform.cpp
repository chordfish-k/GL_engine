#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "engine/core/Transform.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale, float rotation,
                     glm::vec2 offset)
    :position(position), scale(scale), rotation(rotation), offset(offset){}


Transform Transform::Copy() const {
    return Transform(position, scale, rotation, offset);
}

void Transform::CopyTo(Transform &to) const {
    to.position = position;
    to.scale = scale;
    to.rotation = rotation;
    to.offset = offset;
}

Transform &Transform::operator=(const Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    this->rotation = t.rotation;
    this->offset = t.offset;
    return *this;
}

bool Transform::Equals(Transform &t) {
    return t.position == this->position &&
           t.scale == this->scale &&
           t.rotation == this->rotation &&
           t.offset == this->offset;
}

