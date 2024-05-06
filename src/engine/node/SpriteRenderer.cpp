#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/renderer/DebugDraw.hpp"

SpriteRenderer::SpriteRenderer() {}

SpriteRenderer::~SpriteRenderer() {}

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

    // 画外包围盒
    if (PropertiesWindow::GetActiveNode() == this) {
        auto scale = t.scale;
        auto position = t.position;
        auto d = GetSize();// * scale;
        auto center = IsCentered() ? glm::vec2(0, 0) : d * 0.5f;
        auto pos = GetModelMatrix() * (glm::vec4(center.x, center.y, 0, 1));
        DebugDraw::AddBox2D(glm::vec2(pos.x, pos.y), d * scale * 0.5f, t.rotation);
    }

    Node::Update(dt);
}

SpriteRenderer *SpriteRenderer::SetSprite(Sprite *sprite) {
    if (this->sprite == sprite) return this;
    this->sprite = sprite;
    this->isDirty = true;
    return this;
}

SpriteRenderer *SpriteRenderer::SetColor(glm::vec4 color) {
    if (this->color == color) return this;
    this->color = color;
    this->isDirty = true;
    return this;
}

SpriteRenderer *SpriteRenderer::SetOffset(glm::vec2 offset) {
    if (this->offset == offset) return  this;
    this->offset = offset;
    this->isDirty = true;
    return this;
}

bool SpriteRenderer::IsCentered() {
    return centered;
}

SpriteRenderer *SpriteRenderer::SetCentered(bool centered) {
    if (this->centered == centered) return this;
    this->centered = centered;
    this->isDirty = true;
    return this;
}

SpriteRenderer *SpriteRenderer::SetTexture(Texture *texture) {
    if (this->sprite->GetTexture() == texture) return this;
    this->sprite->SetTexture(texture);
    this->isDirty = true;
    return this;
}

json SpriteRenderer::Serialize() {
    json j = Node::Serialize();
    j["data"]["centered"] = IsCentered();
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

    auto &ce = data["centered"];
    if (!ce.empty())
        SetCentered(ce);

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
