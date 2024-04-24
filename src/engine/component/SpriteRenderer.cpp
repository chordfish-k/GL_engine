#include "engine/component/SpriteRenderer.hpp"
#include <glm/ext/vector_float4.hpp>

SpriteRenderer::SpriteRenderer() {
    this->color = glm::vec4();
}

SpriteRenderer::SpriteRenderer(glm::vec4 color) {
    this->color = color;
}

void SpriteRenderer::Start() {}

void SpriteRenderer::Update(float dt) {}