#pragma once

#include "engine/core/GameObject.hpp"
#include <vector>

#define MAX_BATCH_SIZE 1000

class SpriteRenderer;
class RenderBatch;
class GameObject;

class Renderer {
private:
    std::vector<RenderBatch *> batches;

public:
    Renderer();

    ~Renderer();

    // 将GameObject添加到渲染器
    void Add(GameObject *go);

    void Render();

private:
    void Add(SpriteRenderer *sprite);
};