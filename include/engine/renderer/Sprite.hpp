#pragma once

#include "engine/renderer/Texture.hpp"
#include "engine/core/ASerializableObj.hpp"
#include "engine/core/AGuiObj.hpp"
#include <glm/ext/vector_float2.hpp>
#include <rttr/registration>
#include <vector>

class SpriteRenderer;

class Sprite : public ASerializableObj{
    friend class SpriteRenderer;
private:
    float width, height;
    std::vector<glm::vec2> texCoords = {glm::vec2(1, 1), glm::vec2(1, 0),
                                        glm::vec2(0, 0), glm::vec2(0, 1)};
    Texture *texture = nullptr;

public:
    json Serialize() override;

    Sprite *Deserialize(json j) override;

    // 属性
    void SetTexture(Texture *texture);

    void SetTexCoordsToFullTexture();

    void SetTexCoords(const std::vector<glm::vec2> &texCoords);

    Texture *GetTexture() { return texture; }

    std::vector<glm::vec2> GetTexCoords() { return texCoords; }

    float GetWidth() const;

    void SetWidth(float width);

    float GetHeight() const;

    void SetHeight(float height);

    glm::vec2 GetSize() {
        return {width, height};
    }

    unsigned int GetTexId() {
        return texture == nullptr ? 0 : texture->GetId();
    }
};