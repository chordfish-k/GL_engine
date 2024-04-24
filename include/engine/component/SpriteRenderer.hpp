#pragma once

#include "engine/component/Component.hpp"
#include <glm/ext/vector_float4.hpp>

class SpriteRenderer : public Component {
private:
    glm::vec4 color;

public:
    SpriteRenderer();

    SpriteRenderer(glm::vec4 color);

    void Start();

    void Update(float dt);

    // 属性
    glm::vec4 GetColor() { return color; };
};