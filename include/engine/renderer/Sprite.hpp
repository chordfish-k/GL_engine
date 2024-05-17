#pragma once

#include "engine/renderer/Texture.hpp"
#include "engine/core/ASerializableObj.hpp"
#include "engine/core/AGuiObj.hpp"
#include <glm/ext/vector_float2.hpp>
#include <rttr/registration>
#include <vector>

class Sprite : public ASerializableObj, public AGuiObj{
    RTTR_ENABLE(AGuiObj)
private:
    float width, height;
    std::vector<glm::vec2> texCoords = {glm::vec2(1, 1), glm::vec2(1, 0),
                                        glm::vec2(0, 0), glm::vec2(0, 1)};
    Texture *texture = nullptr;

public:
    json Serialize() override;

    Sprite *Deserialize(json j) override;

    // 属性
    void SetTexture(Texture *texture) { this->texture = texture; SetTexCoordsToFullTexture();}

    void SetTexCoordsToFullTexture() {
        texCoords[0] = glm::vec2(1, 1);
        texCoords[1] = glm::vec2(1, 0);
        texCoords[2] = glm::vec2(0, 0);
        texCoords[3] = glm::vec2(0, 1);
    }

    void SetTexCoords(std::vector<glm::vec2> texCoords) {
        this->texCoords.assign(texCoords.begin(), texCoords.end());
    }

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

    void Imgui() override;
};