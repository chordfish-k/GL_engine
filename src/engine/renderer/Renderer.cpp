#include "engine/renderer/Renderer.hpp"
#include "engine/renderer/RenderBatch.hpp"

#include <algorithm>

Shader *Renderer::currentShader = nullptr;

Renderer::Renderer() {}

Renderer::~Renderer() {
    for (auto rb : batches) {
        delete rb;
    }
}

// 将GameObject添加到渲染器
void Renderer::Add(SpriteRenderer *sprite) {
    bool added = false;
    // 找到一个有空间的batch，并且zindex也相同，添加进去
    for (RenderBatch *batch : batches) {
        if (batch->HasRoom() && batch->ZIndex() == sprite->GetZIndex()) {
            Texture *tex = sprite->GetTexture();
            // 如果有材质：这一批已经有这个材质，或者有空闲空间(<8材质)，才将这个sprite添加到这一批
            if (tex == nullptr || batch->HasTexture(tex) || batch->HasTextureRoom()) {
                batch->AddSprite(sprite);
                added = true;
                break;
            }
        }
    }

    // 如果全部batch都满了，添加新的batch
    if (!added) {
        RenderBatch *newBatch =
            new RenderBatch(MAX_BATCH_SIZE, sprite->GetZIndex(), this);
        newBatch->Start();
        batches.push_back(newBatch);
        newBatch->AddSprite(sprite);
        std::sort(batches.begin(), batches.end(),
                  [&](auto a, auto b) { return (*a) < (*b); });
    }
}

void Renderer::Remove(SpriteRenderer *spr) {
    for (auto batch : batches) {
        if (batch->DestroyIfExists(spr)) {
            return;
        }
    }
}

void Renderer::DestroyNode(Node *node) {
    if (!node) return;
    for (auto n : node->children) {
        DestroyNode(n);
    }

    if (dynamic_cast<SpriteRenderer*>(node) == nullptr) return;
    for (auto batch : batches) {
        if (batch->DestroyIfExists(node)) {
            return;
        }
    }
}

void Renderer::Render() {
    currentShader->Use();
    for (int i = 0; i < batches.size(); i++) {
        batches[i]->Render();
    }
}

void Renderer::BindShader(Shader *shader) {
    currentShader = shader;
}

Shader *Renderer::GetBoundShader() {
    return currentShader;
}
