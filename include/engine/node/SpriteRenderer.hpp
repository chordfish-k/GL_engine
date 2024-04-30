#pragma once

#include "engine/node/Sprite.hpp"
#include "engine/node/Node.hpp"
#include "engine/core/Transform.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration.h>
#include <imgui.h>

class Texture;

COMPONENT(SpriteRenderer)
private:
    glm::vec4 color = glm::vec4(1,1,1,1);
    Sprite *sprite = new Sprite();
    Transform lastTransform;
    bool isDirty = true; // 脏标记，表示数据有变化

public:
    SpriteRenderer();

    void Start() override;

    void Update(float dt) override;

    // 属性
    glm::vec4 GetColor() { return color; }

    SpriteRenderer *SetColor(glm::vec4 color);

    Texture *GetTexture() { return sprite->GetTexture(); }

    SpriteRenderer *SetTexture(Texture *texture);

    std::vector<glm::vec2> GetTexCoords() { return sprite->GetTexCoords(); }

    SpriteRenderer *SetSprite(Sprite *sprite);

    bool IsDirty() const { return isDirty; }

    void SetClean() { isDirty = false; }

    void Imgui();

    json Serialize() override;

    SpriteRenderer *Deserialize(json j) override;
};
