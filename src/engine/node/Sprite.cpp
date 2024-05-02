#include "engine/node/Sprite.hpp"
#include "engine/util/Print.hpp"
#include "engine/util/AssetPool.hpp"

// Sprite::Sprite(Texture *texture) {
//     this->texture = texture;
//     this->texCoords = {
//         glm::vec2(1, 1),
//         glm::vec2(1, 0),
//         glm::vec2(0, 0),
//         glm::vec2(0, 1),
//     };
// }

// // 可以选取材质的某一区域(材质UV)作为贴图
// Sprite::Sprite(Texture *texture, std::vector<glm::vec2> texCoords) {
//     this->texture = texture;
//     this->texCoords = texCoords;
// }

Sprite::~Sprite() {
    // if (texture != nullptr)
    //     delete texture;
}

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
