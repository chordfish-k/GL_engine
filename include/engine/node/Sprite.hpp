#pragma once

#include "engine/renderer/Texture.hpp"
#include "engine/core/ASerializableObj.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>

class Sprite : ASerializableObj{
private:
    float width, height;
    std::vector<glm::vec2> texCoords = {glm::vec2(1, 1), glm::vec2(1, 0),
                                        glm::vec2(0, 0), glm::vec2(0, 1)};
    Texture *texture = nullptr;

public:
    // Sprite(Texture *texture);

    // // 可以选取材质的某一区域(材质UV)作为贴图
    // Sprite(Texture *texture, std::vector<glm::vec2> texCoords);

    ~Sprite();

    json Serialize() override;

    Sprite *Deserialize(json j) override;

    // 属性
    void SetTexture(Texture *texture) { this->texture = texture; }

    void SetTexCoords(std::vector<glm::vec2> texCoords) {
        this->texCoords.assign(texCoords.begin(), texCoords.end());
    }

    Texture *GetTexture() { return texture; }

    std::vector<glm::vec2> GetTexCoords() { return texCoords; }

    float GetWidth() const;

    void SetWidth(float width);

    float GetHeight() const;

    void SetHeight(float height);

    unsigned int GetTexId() {
        return texture == nullptr ? 0 : texture->GetId();
    }
};