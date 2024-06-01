#pragma once

#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>


class TransformMatBuilder {
private:
    glm::mat4 mat = glm::mat4(1);
public:
    TransformMatBuilder() = default;
    TransformMatBuilder &Translate(const glm::vec2 &t);
    TransformMatBuilder &Rotate(const float &r);
    TransformMatBuilder &Scale(const glm::vec2 &s);
    glm::mat4 Build();
};

namespace util {
    glm::vec2 TransformPoint(const glm::mat4 &mat, const glm::vec2 &p);
};