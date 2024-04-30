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
    this->sprite = sprite;
    this->isDirty = true;
    return this;
}

SpriteRenderer *SpriteRenderer::SetColor(glm::vec4 color) {
    if (this->color != color) {
        this->color = color;
        this->isDirty = true;
    }
    return this;
}

SpriteRenderer *SpriteRenderer::SetTexture(Texture *texture) {
    this->sprite->SetTexture(texture);
    return this;
}

json SpriteRenderer::Serialize() {
    json j = Node::Serialize();
    j["data"]["color"] = {color.x, color.y, color.z, color.w};
    j["data"]["sprite"] = sprite->Serialize();
    return j;
}

SpriteRenderer *SpriteRenderer::Deserialize(json j) {
    Node::Deserialize(j);
    auto &data = j["data"];
    if (!data.empty()) {
        Node::Deserialize(data);

        auto &c = data["color"];
        if (!c.empty())
            SetColor(glm::vec4(c[0], c[1], c[2], c[3]));

        auto &sp = data["sprite"];
        auto *sprite = new Sprite();
        if (!sp.empty())
            sprite->Deserialize(sp);

        SetSprite(sprite);
    }

    return this;
}

void SpriteRenderer::Imgui() {
    Node::Imgui<SpriteRenderer>();
}
