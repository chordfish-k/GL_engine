#include "engine/node/SpriteRenderer.hpp"
#include <glm/ext/vector_float2.hpp>

SpriteRenderer::SpriteRenderer() {}

void SpriteRenderer::Start() {
    lastTransform = GetTransform().Copy();
    Window::GetScene()->GetRenderer()->Add(this);
    Node::Start();
}

void SpriteRenderer::Update(float dt) {
    Transform t = GetTransform();
    if (!lastTransform.Equals(t)) {
        GetTransform().CopyTo(lastTransform);
        isDirty = true;
    }
    Node::Update(dt);
}

SpriteRenderer *SpriteRenderer::SetSprite(Sprite *sprite) {
    if (this->sprite != sprite) {
        this->sprite = sprite;
        this->isDirty = true;
    }
    return this;
}

SpriteRenderer *SpriteRenderer::SetColor(glm::vec4 color) {
    if (this->color != color) {
        this->color = color;
        this->isDirty = true;
    }
    return this;
}

SpriteRenderer *SpriteRenderer::SetOffset(glm::vec2 offset) {
    if (this->offset != offset) {
        this->offset = offset;
        this->isDirty = true;
    }
    return this;

}

SpriteRenderer *SpriteRenderer::SetTexture(Texture *texture) {
    if (this->sprite->GetTexture() != texture) {
        this->sprite->SetTexture(texture);
        this->isDirty = true;
    }
    return this;
}

json SpriteRenderer::Serialize() {
    json j = Node::Serialize();
    j["data"]["color"] = {color.x, color.y, color.z, color.w};
    j["data"]["offset"] = {offset.x, offset.y};
    j["data"]["sprite"] = sprite->Serialize();
    return j;
}

SpriteRenderer *SpriteRenderer::Deserialize(json j) {
    Node::Deserialize(j);
    auto &data = j["data"];
    if (data.empty()) return this;

    Node::Deserialize(data);

    auto &c = data["color"];
    if (!c.empty())
        SetColor(glm::vec4(c[0], c[1], c[2], c[3]));

    auto &o = data["offset"];
    if (!o.empty())
        SetOffset(glm::vec2(o[0], o[1]));

    auto &sp = data["sprite"];
    auto *sprite = new Sprite();
    if (!sp.empty())
        sprite->Deserialize(sp);

    SetSprite(sprite);

    return this;
}

void SpriteRenderer::Imgui() {
    Node::Imgui<SpriteRenderer>();
}
