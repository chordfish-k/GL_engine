#pragma once

#include <glm/vec2.hpp>

class Rect {
public:
    float x, y, w, h;

public:
    Rect(glm::vec2 pos, glm::vec2 size);

    Rect(float x, float y, float w, float h);

    bool IsInRect(glm::vec2 pos);
};
