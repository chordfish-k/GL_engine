﻿#pragma once

#include "engine/renderer/Sprite.hpp"
#include "engine/node/Node.hpp"
#include "engine/core/Transform.hpp"
#include "engine/renderer/Color.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration.h>
#include <imgui.h>

class Texture;

COMPONENT(SpriteRenderer)
private:
    Color color = {1, 1, 1, 1};
    Sprite *sprite = new Sprite();
    glm::vec2 offset = glm::vec2(0, 0);
    Transform lastTransform;
    bool centered = true; // 从纹理的正中心开始算offset
    bool isDirty = true; // 脏标记，表示数据有变化

public:
    SpriteRenderer();

    ~SpriteRenderer();

    void Start() override;

    void Update(float dt) override;

    // 属性
    Color GetColor() { return color; }

    SpriteRenderer *SetColor(Color color);

    bool IsCentered();

    SpriteRenderer *SetCentered(bool centered);

    glm::vec2 GetOffset() {return offset; };

    SpriteRenderer *SetOffset(glm::vec2 offset);

    glm::vec2 GetSize() {return sprite->GetSize();}

    Texture *GetTexture() { return sprite->GetTexture(); }

    SpriteRenderer *SetTexture(Texture *texture);

    std::vector<glm::vec2> GetTexCoords() { return sprite->GetTexCoords(); }

    Sprite *GetSprite() {return sprite;};

    SpriteRenderer *SetSprite(Sprite *sprite);

    bool IsDirty() const { return isDirty; }

    void SetDirty() { isDirty = true; }

    void SetClean() { isDirty = false; }

    void Imgui() override;

    json Serialize() override;

    SpriteRenderer *Deserialize(json j) override;
};
