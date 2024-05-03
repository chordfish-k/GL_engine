#pragma once

//#include "engine/node/Node.hpp"
#include <vector>
#include "Shader.hpp"

#define MAX_BATCH_SIZE 1000

class SpriteRenderer;
class RenderBatch;
class Node;

class Renderer {
private:
    std::vector<RenderBatch *> batches;

    static Shader *currentShader;

public:
    Renderer();

    ~Renderer();

    // 将Node添加到渲染器

    void Render();

    void Add(SpriteRenderer *sprite);

    void DestroyNode(Node *node);

    static void BindShader(Shader *shader);

    static Shader *GetBoundShader();

};