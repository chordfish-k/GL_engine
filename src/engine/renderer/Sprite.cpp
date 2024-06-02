#include "engine/renderer/Sprite.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/core/MainWindow.hpp"
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

void Sprite::SetTexture(Texture *texture) {
    if (texture != nullptr) {
//        if (this->texture) delete this->texture;
        this->texture = texture;
        this->width = (float) texture->GetWidth();
        this->height = (float) texture->GetHeight();
    }
}

void Sprite::SetTexCoordsToFullTexture() {
    texCoords[0] = glm::vec2(1, 1);
    texCoords[1] = glm::vec2(1, 0);
    texCoords[2] = glm::vec2(0, 0);
    texCoords[3] = glm::vec2(0, 1);
}

void Sprite::SetTexCoords(const std::vector<glm::vec2> &texCoords) {
    this->texCoords.assign(texCoords.begin(), texCoords.end());
}
