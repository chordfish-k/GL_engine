#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>

class Transform {
public:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation = 0.f;

public:
    Transform(glm::vec2 position = {0, 0},
              glm::vec2 scale = {1, 1},
              float rotation = 0.f);

    Transform Copy() const;

    void CopyTo(Transform &to) const;

    Transform &operator=(const Transform &t);

    bool Equals(Transform &t);

    bool Imgui();

    glm::mat4 GetLocalMatrix() const;

    void ApplyDataByLocalMatrix(const glm::mat4 &mat);

    Transform operator+(Transform b) const;


};

