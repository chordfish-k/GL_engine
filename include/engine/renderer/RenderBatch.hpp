#pragma once

#include "engine/component/SpriteRenderer.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include <vector>

class RenderBatch {
private:
    const int VERTEX_POS_SIZE = 2;
    const int VERTEX_COLOR_SIZE = 4;
    const int VERTEX_TEX_COORDS_SIZE = 2;
    const int VERTEX_TEX_ID_SIZE = 1;

    const int VERTEX_POS_OFFSET = 0;
    const int VERTEX_COLOR_OFFSET =
        VERTEX_POS_OFFSET + VERTEX_POS_SIZE * sizeof(float);
    const int VERTEX_TEX_COORDS_OFFSET =
        VERTEX_COLOR_OFFSET + VERTEX_COLOR_SIZE * sizeof(float);
    const int VERTEX_TEX_ID_OFFSET =
        VERTEX_TEX_COORDS_OFFSET + VERTEX_TEX_COORDS_SIZE * sizeof(float);

    const int VERTEX_SIZE = 9;
    const int VERTEX_SIZE_BYTES = VERTEX_SIZE * sizeof(float);

private:
    std::vector<SpriteRenderer *> sprites; // Sprite组件数组
    bool hasRoom;                          // 该批次是否有剩余空间
    std::vector<float> vertices;           // 数据顶点数组
    int verticesSize;
    int maxBatchSize;

    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<Texture *> textures;

    unsigned int vaoID, vboID;
    Shader *shader = nullptr;
    int zIndex; // 用于确定渲染顺序

public:
    RenderBatch(int maxBatchSize, int zIndex);

    ~RenderBatch();

    void Start();

    void AddSprite(SpriteRenderer *spr);

    void Render();

    // 属性
    bool HasRoom() { return hasRoom; }

    int ZIndex() { return zIndex; }

    int operator<(RenderBatch &to) {
        return zIndex < to.zIndex ? -1 : (zIndex == to.zIndex ? 0 : 1);
    }

private:
    void LoadVertexProperties(int index);

    void LoadElementProperties(std::vector<unsigned int> &element, int index);

    std::vector<unsigned int> GenerateIndices();
};