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

SpriteRenderer::~SpriteRenderer() {
    delete lastTransform;
    delete lastParentTransform;
}

void SpriteRenderer::Start() {
    lastTransform = gameObject->transform->Copy();
    lastParentTransform = gameObject->transform->parent ? gameObject->transform->parent : new Transform();
}

void SpriteRenderer::Update(float dt) {
    if (!lastTransform->Equals(*gameObject->transform) || !lastParentTransform ||
        !lastParentTransform->Equals(*gameObject->transform)) {
        gameObject->transform->CopyTo(lastTransform);
        if (gameObject->transform->parent)
            gameObject->transform->parent->CopyTo(lastParentTransform);
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

json SpriteRenderer::Serialize() {
    json j;
    j["component"] = GetComponentName();
    j["uid"] = GetUid();
    j["color"] = {color.x, color.y, color.z, color.w};
    j["offset"] = {offset.x, offset.y};
    j["sprite"] = sprite->Serialize();
    return j;
}

SpriteRenderer *SpriteRenderer::Deserialize(json j) {
    Component::Deserialize(j);

    auto &c = j["color"];
    if (!c.empty()) SetColor(glm::vec4(c[0], c[1], c[2], c[3]));

    auto &o = j["offset"];
    if (!o.empty()) SetOffset(glm::vec2(o[0], o[1]));

    auto *sprite = new Sprite();
    auto &s = j["sprite"];
    if (!s.empty()) sprite->Deserialize(s);
    SetSprite(sprite);

    return this;
}

void SpriteRenderer::Imgui() {
    Component::Imgui<SpriteRenderer>();
}


