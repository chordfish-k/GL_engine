#pragma once

#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"

class Prefabs {
public:
    static Node *GenerateSpriteNode(Sprite *sprite, glm::vec2 scale) {
        auto *block = new SpriteRenderer();
        block->SetName("Sprite_Gen");
        block->SetScale(scale);
        block->SetSprite(sprite);
        return block;
    }
};
