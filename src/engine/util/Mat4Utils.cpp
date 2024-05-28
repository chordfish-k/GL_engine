#include <glm/ext/matrix_transform.hpp>
#include "engine/util/Mat4Utils.hpp"



TransformMatBuilder &TransformMatBuilder::Translate(const glm::vec2 &t) {
    mat = glm::translate(mat, {t, 0});
    return *this;
}

TransformMatBuilder &TransformMatBuilder::Rotate(const float &rad) {
    mat = glm::rotate(mat, rad, {0, 0, 1});
    return *this;
}

TransformMatBuilder &TransformMatBuilder::Scale(const glm::vec2 &s) {
    mat = glm::scale(mat, {s, 1});
    return *this;
}

glm::mat4 TransformMatBuilder::Build() {
    return mat;
}
