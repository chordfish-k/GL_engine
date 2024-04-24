#pragma once

#include "engine/component/SpriteRenderer.hpp"
#include "engine/renderer/Shader.hpp"
#include <vector>

class RenderBatch {
private:
    const int VERTEX_POS_SIZE = 2;
    const int VERTEX_COLOR_SIZE = 4;

    const int VERTEX_POS_OFFSET = 0;
    const int VERTEX_COLOR_OFFSET =
        VERTEX_POS_OFFSET + VERTEX_POS_SIZE * sizeof(float);

    const int VERTEX_SIZE = 6;
    const int VERTEX_SIZE_BYTES = VERTEX_SIZE * sizeof(float);

private:
    std::vector<SpriteRenderer *> sprites; // Sprite组件数组
    bool hasRoom;                          // 该批次是否有剩余空间
    std::vector<float> vertices;           // 数据顶点数组
    int verticesSize;
    int maxBatchSize;

    unsigned int vaoID, vboID;
    Shader *shader;

public:
    RenderBatch(int maxBatchSize);

    ~RenderBatch();

    void Start();

    void AddSprite(SpriteRenderer *spr);

    void Render();

    // 属性
    bool HasRoom() { return hasRoom; }

private:
    void LoadVertexProperties(int index);

    void LoadElementProperties(std::vector<unsigned int> &element, int index);

    std::vector<unsigned int> GenerateIndices();
};