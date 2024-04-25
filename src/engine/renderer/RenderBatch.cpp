#include <algorithm>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vector>

#include "engine/renderer/RenderBatch.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/core/Window.hpp"
#include "engine/core/Camera.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Common.hpp"
#include "engine/util/Print.hpp"

RenderBatch::RenderBatch(int maxBatchSize) {
    shader = AssetPool::GetShader("assets/shader/default.glsl");
    shader->Compile();

    this->maxBatchSize = maxBatchSize;

    // 二维引擎，渲染四边形
    verticesSize = maxBatchSize * 4 * VERTEX_SIZE;
    vertices = std::vector<float>(verticesSize, 0.f);

    sprites.clear();
    hasRoom = true;
}

RenderBatch::~RenderBatch() {
    // delete shader;
}

void RenderBatch::Start() {
    // ********************************
    // 创建VAO, VBO, EBO 缓冲， 并发送到GPU
    // ********************************

    // 生成并绑定VAO
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // 创建VBO GL_DYNAMIC_DRAW:每次绘制数据都会改变
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    // 创建并上传标记缓冲区，GL_STATIC_DRAW:每次绘制数据都不变，静态数据
    unsigned int eboID;
    glGenBuffers(1, &eboID);
    std::vector<unsigned int> indices = GenerateIndices();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_SIZE_BYTES, (void *)(VERTEX_POS_OFFSET));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_SIZE_BYTES, (void *)(VERTEX_COLOR_OFFSET));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, VERTEX_TEX_COORDS_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_SIZE_BYTES,
                          (void *)(VERTEX_TEX_COORDS_OFFSET));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, VERTEX_TEX_ID_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_SIZE_BYTES, (void *)(VERTEX_TEX_ID_OFFSET));
    glEnableVertexAttribArray(3);
}

void RenderBatch::AddSprite(SpriteRenderer *spr) {
    // 添加SpriteRenderer并获取索引
    sprites.push_back(spr);

    // 将纹理添加到数组
    if (spr->GetTexture() != nullptr) {
        if (textures.end() ==
            std::find(textures.begin(), textures.end(), spr->GetTexture())) {
            textures.push_back(spr->GetTexture());
        }
    }

    // 将属性添加到本地顶点数组
    LoadVertexProperties(sprites.size() - 1);

    // 到达batch上限
    if (sprites.size() >= maxBatchSize) {
        hasRoom = false;
    }
}

void RenderBatch::Render() {
    // 每帧重新缓冲所有数据 glBufferSubData：用来更新VBO中的部分数据
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
                    vertices.data());

    // 使用shader
    shader->Use();
    shader->UploadMat4("uProjection",
                       Window::GetScene()->GetCamera()->GetProjMatrix());
    shader->UploadMat4("uView",
                       Window::GetScene()->GetCamera()->GetViewMatrix());
    // 启用纹理
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i + 1); // 下标0表示无texture占位
        textures[i]->Bind();
    }
    shader->UploadIntArray("uTextures", util::LenOf(texSlots), texSlots);

    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, sprites.size() * 6, GL_UNSIGNED_INT, 0);

    // 解绑
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 取消纹理
    for (int i = 0; i < textures.size(); i++) {
        textures[i]->Unbind();
    }
    shader->Detach();
}

void RenderBatch::LoadVertexProperties(int index) {
    auto sprite = sprites[index];

    // 每个sprite4个顶点
    int offset = index * 4 * VERTEX_SIZE;
    // float float      float float float float
    glm::vec4 color = sprite->GetColor();
    std::vector<glm::vec2> texCoords = sprite->GetTexCoords();

    int texId = 0; // 0 表示无texture
    if (sprite->GetTexture() != nullptr) {
        for (int i = 0; i < textures.size(); i++) {
            if (textures[i] == sprite->GetTexture()) {
                texId = i + 1;
                break;
            }
        }
    }

    // 根据相关属性添加顶点
    float xAdd = 1.f;
    float yAdd = 1.f;
    for (int i = 0; i < 4; i++) {
        if (i == 1) {
            yAdd = 0.f;
        } else if (i == 2) {
            xAdd = 0.f;
        } else if (i == 3) {
            yAdd = 1.f;
        }

        // 载入位置信息，根据为题中心进行偏移
        auto tr = sprite->gameObject->transform;
        vertices[offset + 0] = tr->position.x + (xAdd * tr->scale.x);
        vertices[offset + 1] = tr->position.y + (yAdd * tr->scale.y);

        // 载入颜色信息
        vertices[offset + 2] = color.x;
        vertices[offset + 3] = color.y;
        vertices[offset + 4] = color.z;
        vertices[offset + 5] = color.w;

        // 载入材质坐标
        vertices[offset + 6] = texCoords[i].x;
        vertices[offset + 7] = texCoords[i].y;

        // 载入材质id
        vertices[offset + 8] = texId;

        offset += VERTEX_SIZE;
    }
}

void RenderBatch::LoadElementProperties(std::vector<unsigned int> &element,
                                        int index) {
    int offsetArrayIndex = 6 * index;
    int offset = 4 * index;

    // 3, 2, 0, 0, 2, 1         7, 6, 4, 4, 6, 5
    // 三角形A
    element[offsetArrayIndex + 0] = offset + 3;
    element[offsetArrayIndex + 1] = offset + 2;
    element[offsetArrayIndex + 2] = offset + 0;

    // 三角形B
    element[offsetArrayIndex + 3] = offset + 0;
    element[offsetArrayIndex + 4] = offset + 2;
    element[offsetArrayIndex + 5] = offset + 1;
}

std::vector<unsigned int> RenderBatch::GenerateIndices() {
    // 绘制每个四边形要6个顶点（每个三角形3个，有2个重复顶点）
    std::vector<unsigned int> elements =
        std::vector<unsigned int>(6 * maxBatchSize);
    for (int i = 0; i < maxBatchSize; i++) {
        LoadElementProperties(elements, i);
    }
    return elements;
}