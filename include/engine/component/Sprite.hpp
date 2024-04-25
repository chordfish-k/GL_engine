#pragma once

#include "engine/renderer/Texture.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>
class Sprite {
private:
    std::vector<glm::vec2> texCoords;
    Texture *texture = nullptr;

public:
    Sprite(Texture *texture);

    // 可以选取材质的某一区域(材质UV)作为贴图
    Sprite(Texture *texture, std::vector<glm::vec2> texCoords);

    ~Sprite();

    // 属性
    Texture *GetTexture() { return texture; }

    std::vector<glm::vec2> GetTexCoords() { return texCoords; }
};