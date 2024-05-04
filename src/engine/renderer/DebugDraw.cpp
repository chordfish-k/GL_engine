#include <glad/glad.h>
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/Window.hpp"
#include "engine/core/Camera.hpp"
#include "engine/util/AssetPool.hpp"

std::vector<Line2D> DebugDraw::lines;

float DebugDraw::vertexArray[DEBUG_MAX_LINES * 6 * 2];

Shader *DebugDraw::shader = nullptr;

unsigned int DebugDraw::vaoID = 0;
unsigned int DebugDraw::vboID = 0;

bool DebugDraw::started = false;


void DebugDraw::Start() {
    // 生成vao
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // 创建vbo
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, (long) (DEBUG_MAX_LINES * 6 * 2) * sizeof(float),
                 vertexArray, GL_DYNAMIC_DRAW);

    // 启用顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float),
                          reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 设置线宽
    glLineWidth(2.0f);

    // 获取Shader
    shader = AssetPool::GetShader("assets/shader/debugLine2D.glsl");
}

void DebugDraw::BeginFrame() {
    if (!started) {
        Start();
        started = true;
    }

    // 移除到达lifetime的线
    for (int i=0; i < lines.size(); i++) {
        if (lines[i].BeginFrame() < 0) {
            lines.erase(lines.begin()+i);
            i--;
        }
    }
}

void DebugDraw::Draw() {
    if (lines.size() == 0) return;

    int index = 0;
    for (Line2D line : lines) {
        for (int i = 0; i < 2; i++) {
            glm::vec2 position = i == 0 ? line.GetFrom() : line.GetTo();
            glm::vec3 color = line.GetColor();

            // 载入坐标
            vertexArray[index + 0] = position.x;
            vertexArray[index + 1] = position.y;
            vertexArray[index + 2] = -10.0f;

            // 载入颜色
            vertexArray[index + 3] = color.x;
            vertexArray[index + 4] = color.y;
            vertexArray[index + 5] = color.z;
            index += 6;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * 6 * 2 * sizeof(float), vertexArray);

    // 使用shader
    shader->Use();
    shader->UploadMat4("uProjection", Window::GetScene()->GetCamera()->GetProjMatrix());
    shader->UploadMat4("uView", Window::GetScene()->GetCamera()->GetViewMatrix());
    // 绑定vao
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 绘画
    glDrawArrays(GL_LINES, 0, lines.size() * 6 * 2);

    // 解绑
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

    shader->Detach();
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to) {
    AddLine2D(from, to, {0, 1, 0}, 1);
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec3 color) {
    AddLine2D(from, to, color, 1);
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec3 color,
                          int lifeTime) {
    if (lines.size() >= DEBUG_MAX_LINES) return;
    DebugDraw::lines.emplace_back(from, to, color, lifeTime);
}
