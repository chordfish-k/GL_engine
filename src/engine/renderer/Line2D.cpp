#include "engine/renderer/Line2D.hpp"

int Line2D::BeginFrame() {
    this->lifeTime--;
    return this->lifeTime;
}
