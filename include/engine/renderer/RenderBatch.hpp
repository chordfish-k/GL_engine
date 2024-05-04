#pragma once

#include "engine/node/SpriteRenderer.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include <vector>

#define VERTEX_POS_SIZE 2
#define VERTEX_COLOR_SIZE 4
#define VERTEX_TEX_COORDS_SIZE 2
#define VERTEX_TEX_ID_SIZE 1
#define VERTEX_POS_OFFSET 0
#define VERTEX_COLOR_OFFSET  (VERTEX_POS_OFFSET + VERTEX_POS_SIZE * sizeof(float))
#define VERTEX_TEX_COORDS_OFFSET (VERTEX_COLOR_OFFSET + VERTEX_COLOR_SIZE * sizeof(float))
#define VERTEX_TEX_ID_OFFSET (VERTEX_TEX_COORDS_OFFSET + VERTEX_TEX_COORDS_SIZE * sizeof(float))
#define VERTEX_SIZE 9
#define VERTEX_SIZE_BYTES VERTEX_SIZE * sizeof(float)

class RenderBatch {
private:
    Renderer *renderer = nullptr;
    SpriteRenderer **sprites; // Sprite组件数组
    bool hasRoom;                          // 该批次是否有剩余空间
    std::vector<float> vertices;           // 数据顶点数组
    int verticesSize;
    int maxBatchSize;
    int numSprites;

    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<Texture *> textures;

    unsigned int vaoID, vboID;
    int zIndex; // 用于确定渲染顺序

public:
    RenderBatch(int maxBatchSize, int zIndex, Renderer *renderer);

    ~RenderBatch();

    void Start();

    void AddSprite(SpriteRenderer *spr);

    bool DestroyIfExists(Node *pNode);

    void Render();

    // 属性
    bool HasRoom() { return hasRoom; }

    int ZIndex() { return zIndex; }

    bool operator<(RenderBatch &rhs) {
        return zIndex < rhs.zIndex;
    }

    bool HasTexture(Texture *pTexture);

    bool HasTextureRoom();

private:
    void LoadVertexProperties(int index);

    void LoadElementProperties(std::vector<unsigned int> &element, int index);

    std::vector<unsigned int> GenerateIndices();
};

