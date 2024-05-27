#pragma once

#include <glm/vec2.hpp>
class Linear {
public:
    glm::vec2 velocity = {0, 0};
    float damp = 0.9f; // 线性阻尼
public:
    Linear() {}
    Linear(glm::vec2 velocity, float damp);

    bool Imgui();
};
