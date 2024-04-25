#pragma once

#include "engine/component/Component.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

class Texture;

class SpriteRenderer : public Component {
private:
    glm::vec4 color;
    glm::vec2 texCoords[4];
    Texture *texture = nullptr;

public:
    SpriteRenderer();

    SpriteRenderer(glm::vec4 color);

    SpriteRenderer(Texture *texture);
    
    SpriteRenderer(glm::vec4 color, Texture *texture);

    void Start();

    void Update(float dt);

    // 属性
    glm::vec4 GetColor() { return color; }

    Texture *GetTexture() { return texture; }

    glm::vec2 *GetTexCoords() { return texCoords; }
};