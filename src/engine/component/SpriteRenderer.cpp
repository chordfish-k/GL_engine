#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Sprite.hpp"
#include "engine/core/GameObject.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

// SpriteRenderer::SpriteRenderer() : SpriteRenderer(glm::vec4()) {}

// SpriteRenderer::SpriteRenderer(glm::vec4 color)
//     : color(glm::vec4()), sprite(new Sprite(nullptr)) {}

// SpriteRenderer::SpriteRenderer(Sprite *sprite)
//     : color(glm::vec4(1, 1, 1, 1)), sprite(sprite) {}

void SpriteRenderer::Start() {
    lastTransform = gameObject->transform->Copy();
}

void SpriteRenderer::Update(float dt) {
    if (!lastTransform->Equals(*gameObject->transform)) {
        gameObject->transform->CopyTo(lastTransform);
        isDirty = true;
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
std::string SpriteRenderer::Serialize() {
    json j;
    j["component"] = componentName;
    j["color"] = {color.x, color.y, color.z, color.w};
    j["sprite"] = Str2Json(sprite->Serialize());

    return j.dump(2);
}

SpriteRenderer *SpriteRenderer::Deserialize(json j) {
    auto &c = j["color"];
    SetColor(glm::vec4(c[0], c[1], c[2], c[3]));
    Sprite *sprite = new Sprite();
    sprite->Deserialize(j["sprite"]);
    SetSprite(sprite);
    return this;
}
