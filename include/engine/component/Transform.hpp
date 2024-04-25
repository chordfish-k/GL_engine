#pragma once

#include "Component.hpp"
#include "engine/component/Component.hpp"
#include <glm/ext/vector_float2.hpp>

class Transform : public Component {
public:
    glm::vec2 position;
    glm::vec2 scale;

public:
    Transform() : Transform(glm::vec2(), glm::vec2()) {}

    Transform(glm::vec2 position) : Transform(position, glm::vec2()) {}

    Transform(glm::vec2 position, glm::vec2 scale);

    void Init();

    void Start();

    void Update(float dt);

    Transform *Copy();

    void CopyTo(Transform *to);

    Transform &operator=(Transform &t);

    bool Equals(Transform &t);
};