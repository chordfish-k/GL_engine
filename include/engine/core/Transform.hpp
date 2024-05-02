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

    void Imgui();

    Transform operator+(Transform b) const {
        Transform res;
        res.position = position + b.position;
        res.scale = scale * b.scale;
        res.rotation = rotation + b.rotation;
        return res;
    }
};

