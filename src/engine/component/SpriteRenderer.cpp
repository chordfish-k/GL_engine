#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Sprite.hpp"
#include "engine/core/GameObject.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

SpriteRenderer::SpriteRenderer() : SpriteRenderer(glm::vec4()) {}

SpriteRenderer::SpriteRenderer(glm::vec4 color)
    : color(glm::vec4()), sprite(new Sprite(nullptr)) {}

SpriteRenderer::SpriteRenderer(Sprite *sprite)
    : color(glm::vec4(1, 1, 1, 1)), sprite(sprite) {}

void SpriteRenderer::Start() {
    lastTransform = gameObject->transform->Copy();
}

void SpriteRenderer::Update(float dt) {
    if (!lastTransform->Equals(*gameObject->transform)) {
        gameObject->transform->CopyTo(lastTransform);
    }
}

void SpriteRenderer::SetSprite(Sprite *sprite) {
    this->sprite = sprite;
    this->isDirty = true;
}

void SpriteRenderer::SetColor(glm::vec4 color) {
    if (this->color != color) {
        this->color = color;
        this->isDirty = true;
    }
}