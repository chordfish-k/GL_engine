#include "engine/component/Sprite.hpp"
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

    return j;
}

Sprite *Sprite::Deserialize(json j) {
    SetTexture(AssetPool::GetTexture(j["texture"]));
    auto &tc = j["texCoords"];
    std::vector<glm::vec2> list;
    for (auto &t : tc) {
        list.emplace_back(t[0], t[1]);
    }
    SetTexCoords(list);
    return this;
}
