#pragma once

#include "engine/component/Sprite.hpp"
#include "engine/component/Component.hpp"
#include "engine/component/Transform.hpp"
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
    glm::vec2 offset = {0.5, 0.5};
    Transform *lastTransform, *lastParentTransform;
    bool isDirty = true; // 脏标记，表示数据有变化

public:
    // SpriteRenderer();

    // SpriteRenderer(glm::vec4 color);

    // SpriteRenderer(Sprite *sprite);

    ~SpriteRenderer();

    void Start() override;

    void Update(float dt) override;

    // 属性
    glm::vec4 GetColor() { return color; }

    void SetColor(glm::vec4 color);

    glm::vec2 GetOffset(){
        return offset;
    }

    void SetOffset(glm::vec2 offset) {
        this->offset = offset;
    }

    Texture *GetTexture() { return sprite->GetTexture(); }

    void SetTexture(Texture *texture) { this->sprite->SetTexture(texture); }

    std::vector<glm::vec2> GetTexCoords() { return sprite->GetTexCoords(); }

    void SetSprite(Sprite *sprite);

    bool IsDirty() const { return isDirty; }

    void SetClean() { isDirty = false; }

    void Imgui();

    json Serialize() override;

    SpriteRenderer *Deserialize(json j) override;
};
