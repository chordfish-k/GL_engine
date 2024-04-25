#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Sprite.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

SpriteRenderer::SpriteRenderer() : SpriteRenderer(glm::vec4()) {}

SpriteRenderer::SpriteRenderer(glm::vec4 color)
    : color(glm::vec4()) {}

SpriteRenderer::SpriteRenderer(Sprite * sprite)
    : color(glm::vec4(1,1,1,1)), sprite(sprite) {
    new Sprite(nullptr, {glm::vec2(1, 1), glm::vec2(1, 1)});
    }


void SpriteRenderer::Start() {}

void SpriteRenderer::Update(float dt) {}