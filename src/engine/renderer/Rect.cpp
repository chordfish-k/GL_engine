#include "engine/renderer/Rect.hpp"

Rect::Rect(glm::vec2 pos, glm::vec2 size)
    : Rect(pos.x, pos.y, size.x, size.y){}

Rect::Rect(float x, float y, float w, float h)
    :x(x), y(y), w(w), h(h){}

bool Rect::IsInRect(glm::vec2 pos) {
    if (pos.x < x || pos.x > x + w) return false;
    if (pos.y < y || pos.y > y + h) return false;
    return true;
}
