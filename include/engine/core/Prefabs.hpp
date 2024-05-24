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

    static Node *GenerateSpriteRendererFromSpriteSheet(Spritesheet *spritesheet,  int frame) {
        auto *block = new SpriteRenderer();
        block->SetName("Sprite_Gen");
        block->SetSprite(spritesheet->GetSprite(frame));
        block->SetAnimation({spritesheet->GetColumns(), spritesheet->GetRows(), frame});
        return block;
    }
};
