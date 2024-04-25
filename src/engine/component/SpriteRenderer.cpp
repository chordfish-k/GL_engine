#include "engine/component/SpriteRenderer.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

SpriteRenderer::SpriteRenderer() : SpriteRenderer(glm::vec4()) {}

SpriteRenderer::SpriteRenderer(glm::vec4 color)
    : SpriteRenderer(color, nullptr) {}

SpriteRenderer::SpriteRenderer(Texture *texture)
    : SpriteRenderer(glm::vec4(1, 1, 1, 1), texture) {}

SpriteRenderer::SpriteRenderer(glm::vec4 color, Texture *texture) {
    this->color = color;
    this->texture = texture;
    this->texCoords[0] = glm::vec2(1, 1);
    this->texCoords[1] = glm::vec2(1, 0);
    this->texCoords[2] = glm::vec2(0, 0);
    this->texCoords[3] = glm::vec2(0, 1);
}

void SpriteRenderer::Start() {}

void SpriteRenderer::Update(float dt) {}