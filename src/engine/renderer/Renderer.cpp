#include "engine/renderer/RenderBatch.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/util/Print.hpp"
#include <algorithm>
#include <ios>

Renderer::Renderer() {}

Renderer::~Renderer() {
    for (auto rb : batches) {
        if (rb == nullptr)
            continue;
        delete rb;
    }
}

// 将GameObject添加到渲染器
void Renderer::Add(GameObject *go) {
    auto spr = go->GetComponent<SpriteRenderer>();
    if (spr != nullptr) {
        Add(spr);
    }
}

void Renderer::Add(SpriteRenderer *sprite) {
    bool added = false;
    // 找到一个有空间的batch，并且zindex也相同，添加进去
    for (RenderBatch *batch : batches) {
        if (batch->HasRoom() &&
            batch->ZIndex() == sprite->gameObject->ZIndex()) {
            batch->AddSprite(sprite);
            added = true;
            break;
        }
    }

    // 如果全部batch都满了，添加新的batch
    if (!added) {
        RenderBatch *newBatch =
            new RenderBatch(MAX_BATCH_SIZE, sprite->gameObject->ZIndex());
        newBatch->Start();
        batches.push_back(newBatch);
        newBatch->AddSprite(sprite);
        std::sort(batches.begin(), batches.end());
    }
}

void Renderer::Render() {
    for (auto batch : batches) {
        batch->Render();
    }
}
