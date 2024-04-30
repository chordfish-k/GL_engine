#pragma once

//#include "engine/node/Node.hpp"
#include <vector>

#define MAX_BATCH_SIZE 1000

class SpriteRenderer;
class RenderBatch;
class Node;

class Renderer {
private:
    std::vector<RenderBatch *> batches;

public:
    Renderer();

    ~Renderer();

    // 将Node添加到渲染器

    void Render();

    void Add(SpriteRenderer *sprite);
};