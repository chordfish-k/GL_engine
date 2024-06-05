#include <filesystem>
#include "engine/node/SpriteRenderer.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/util/ReflectUtil.hpp"


SpriteRenderer::SpriteRenderer() {}

SpriteRenderer::~SpriteRenderer() {
    delete sprite;
    delete animation;
}

void SpriteRenderer::Start() {
    lastTransform = GetTransform().Copy();
    auto scene = MainWindow::GetScene();
    if (scene != nullptr) {
        scene->GetRenderer()->Add(this);
    }
    Node::Start();
}

void SpriteRenderer::Update(float dt) {
    if (!active) return;

    Transform t = GetTransform();

    // 监测Animation的变化，重设TexCoord
    if (animation->IsModified()) {
        animation->ApplyModifyToSprite(sprite);
        isDirty = true;
        animation->ClearModify();
    }

    if (!lastTransform.Equals(t)) {
        GetTransform().CopyTo(lastTransform);
        isDirty = true;
    }

    Node::Update(dt);
}

void SpriteRenderer::EditorUpdate(float dt) {
    if (!active) return;

    Transform t = GetTransform();

    // 监测Animation的变化，重设TexCoord
    if (animation->IsModified()) {
        animation->ApplyModifyToSprite(sprite);
        isDirty = true;
        animation->ClearModify();
    }

    if (!lastTransform.Equals(t)) {
        GetTransform().CopyTo(lastTransform);
        isDirty = true;
    }

    Node::EditorUpdate(dt);
}

SpriteRenderer *SpriteRenderer::SetSprite(Sprite *sprite) {
    if (this->sprite == sprite) return this;
    delete this->sprite;
    this->sprite = sprite;
    this->isDirty = true;
    return this;
}

SpriteRenderer *SpriteRenderer::SetColor(Color color) {
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
    j["data"]["color"] = {color.r, color.g, color.b, color.a};
    j["data"]["offset"] = {offset.x, offset.y};
    j["data"]["texture"] = sprite->Serialize();
    j["data"]["animation"] = animation->Serialize();
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
        SetColor(Color(c[0], c[1], c[2], c[3]));

    auto &o = data["offset"];
    if (!o.empty())
        SetOffset(glm::vec2(o[0], o[1]));

    auto &sp = data["texture"];
    if (!sp.empty())
        sprite->Deserialize(sp);

    auto &an = data["animation"];
    if (!an.empty())
        animation->Deserialize(an);

    return this;
}

bool SpriteRenderer::IsVisitable() const {
    return visitable;
}

void SpriteRenderer::SetVisitable(bool visitable_) {
    if (visitable != visitable_) {
        visitable = visitable_;
        isDirty = true;
    }
}


void SpriteRenderer::Imgui(){
    Node::Imgui();
    ImGui::Dummy({1, 5});
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(GetNodeType().c_str())) {

        ShowImgui();

        auto te = GetTexture();
        std::string filePath = te ? te->GetFilePath() : "";

        if (MyImGui::DrawResourceDragDropBox("texture",filePath)) {
            auto tex = AssetPool::GetTexture(filePath);
            auto sp = new Sprite();
            sp->SetTexture(tex);

            auto spr = this;
            spr->SetSprite(sp);
            MainWindow::GetScene()->GetRenderer()->Remove(spr);
            MainWindow::GetScene()->GetRenderer()->Add(spr);

            // 重置animation
            animation->Reset();
        }

        animation->Imgui();
    }
}

void SpriteRenderer::SetActive(bool active) {
    Node::SetActive(active);
    MainWindow::GetScene()->GetRenderer()->DestroyNode(this);
    if (active) {
        MainWindow::GetScene()->GetRenderer()->Add(this);
    }
    isDirty = true;
}

void SpriteRenderer::BindThisToScript(sol::table &table) {
    table["this"] = (SpriteRenderer*)this;
}

SpriteRenderer *SpriteRenderer::GetNode() {
    return (SpriteRenderer*)this;
}

bool SpriteRenderer::IsFlipX() const {
    return flipX;
}

void SpriteRenderer::SetFlipX(bool flipX_) {
    if (flipX != flipX_) {
        flipX = flipX_;
        isDirty = true;
    }
}

bool SpriteRenderer::IsFlipY() const {
    return flipY;
}

void SpriteRenderer::SetFlipY(bool flipY_) {
    if (flipY != flipY_) {
        flipY = flipY_;
        isDirty = true;
    }
}

BEGIN_RTTR_REG (SpriteRenderer)
rttr::registration::class_<SpriteRenderer>("SpriteRenderer")
    .constructor<>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )
    .property("centered", &SpriteRenderer::IsCentered, &SpriteRenderer::SetCentered)
    .property("visitable", &SpriteRenderer::IsVisitable, &SpriteRenderer::SetVisitable)
    .property("flipX", &SpriteRenderer::IsFlipX, &SpriteRenderer::SetFlipX)
    .property("flipY", &SpriteRenderer::IsFlipY, &SpriteRenderer::SetFlipY)
    .property("color", &SpriteRenderer::GetColor, &SpriteRenderer::SetColor)
    .property("offset", &SpriteRenderer::GetOffset, &SpriteRenderer::SetOffset)
    .property("sprite", &SpriteRenderer::GetSprite, &SpriteRenderer::SetSprite);
END_RTTR_REG(SpriteRenderer)