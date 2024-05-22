#include "engine/renderer/Sprite.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/util/Print.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/editor/MyImGui.hpp"
#include <filesystem>

json Sprite::Serialize() {
    json j;
    for (int i=0; i<texCoords.size(); i++) {
        j["texCoords"][i] = {texCoords[i].x, texCoords[i].y};
    }
  j["texture"] = texture ? texture->GetFilePath() : "";
  j["size"] = {width, height};

  return j;
}

Sprite *Sprite::Deserialize(json j) {
    auto &t = j["texture"];
    if (!t.empty()) SetTexture(AssetPool::GetTexture(t));

    auto &tc = j["texCoords"];
    if (!tc.empty()) {
        std::vector<glm::vec2> list;
        for (auto &t : tc) {
            if (!t.empty() && t.size() == 2)
                list.emplace_back(t[0], t[1]);
        }
        SetTexCoords(list);
    }

    auto &s = j["size"];
    if (!s.empty() && s.size() == 2) {
        width = s[0];
        height = s[1];
    }

    return this;
}
float Sprite::GetWidth() const {
    return width;
}

void Sprite::SetWidth(float width) {
    Sprite::width = width;
}

float Sprite::GetHeight() const {
    return height;
}

void Sprite::SetHeight(float height) {
    Sprite::height = height;
}

void Sprite::Imgui() {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("sprite")) {

        auto te = GetTexture();
        std::string path = te ? te->GetFilePath() : "";
        std::filesystem::path path1 = path;
        std::string filePath = path1.filename().string();
        if (MyImGui::DrawResourceDragDropBox("sprite",filePath)) {
            auto tex = AssetPool::GetTexture(filePath);
            auto spr = GetSpriteRenderer();
            auto sp = new Sprite();
            sp->SetTexture(tex);
            spr->SetSprite(sp);

            Window::GetScene()->GetRenderer()->Remove(spr);
            Window::GetScene()->GetRenderer()->Add(spr);
        }
    }
}
