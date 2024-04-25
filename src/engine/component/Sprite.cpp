#include "engine/component/Sprite.hpp"

Sprite::Sprite(Texture *texture) {
    this->texture = texture;
    this->texCoords = {
        glm::vec2(1, 1),
        glm::vec2(1, 0),
        glm::vec2(0, 0),
        glm::vec2(0, 1),
    };
}

// 可以选取材质的某一区域(材质UV)作为贴图
Sprite::Sprite(Texture *texture, std::vector<glm::vec2> texCoords) {
    this->texture = texture;
    this->texCoords = texCoords;
}

Sprite::~Sprite() {
    if (texture != nullptr)
        delete texture;
}