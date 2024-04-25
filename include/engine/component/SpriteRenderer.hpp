#pragma once

#include "engine/component/Sprite.hpp"
#include "engine/component/Component.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>

class Texture;

class SpriteRenderer : public Component {
private:
    glm::vec4 color;
    Sprite *sprite;

public:
    SpriteRenderer();

    SpriteRenderer(glm::vec4 color);

    SpriteRenderer(Sprite *sprite);

    void Start();

    void Update(float dt);

    // 属性
    glm::vec4 GetColor() { return color; }

    Texture *GetTexture() { return sprite->GetTexture(); }

    std::vector<glm::vec2> GetTexCoords() { return sprite->GetTexCoords(); }
};