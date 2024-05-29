#pragma once

#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/util/Json.hpp"
#include <rttr/type>

using namespace nlohmann;

class PrefabsUtils {
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


    static Node *GenerateNodeFromJson(json j) {
        if (j.empty()) return nullptr;

        auto &t = j["type"];
        if (!t.empty() || !t.is_string()) return nullptr;

        auto nodeType = rttr::type::get_by_name(t);
        if (!nodeType.is_valid()) return nullptr;

        auto nodeClassPtr = rttr::type::get<Node*>();
        if (!nodeClassPtr.is_valid()) return nullptr;

        auto var = nodeType.create();
        auto nodeVar = var.get_value<Node*>();
        if (!nodeVar) return nullptr;

        nodeVar->Deserialize(j);

        return nodeVar;
    }
};