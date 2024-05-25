#include <glad/glad.h>
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/Camera.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/MainWindow.hpp"

#define DEBUG_DEFAULT_COLOR {1, 0.4, 0, 1}

std::vector<Line2D> DebugDraw::lines;

float DebugDraw::vertexArray[DEBUG_MAX_LINES * 7 * 2];

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
    glBufferData(GL_ARRAY_BUFFER, (long) (DEBUG_MAX_LINES * 7 * 2) * sizeof(float),
                 vertexArray, GL_DYNAMIC_DRAW);

    // 启用顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 7 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, false, 7 * sizeof(float),
                          reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



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
    int index = 0;
    memset(vertexArray, 0, sizeof(vertexArray));

    for (Line2D line : lines) {
        for (int i = 0; i < 2; i++) {
            glm::vec2 position = i == 0 ? line.GetFrom() : line.GetTo();
            glm::vec4 color = line.GetColor();

            // 载入坐标
            vertexArray[index + 0] = position.x;
            vertexArray[index + 1] = position.y;
            vertexArray[index + 2] = -10.0f;

            // 载入颜色
            vertexArray[index + 3] = color.x;
            vertexArray[index + 4] = color.y;
            vertexArray[index + 5] = color.z;
            vertexArray[index + 6] = color.w;
            index += 7;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines.capacity() * 7 * 2 * sizeof(float), vertexArray);

    // 使用shader
    shader->Use();
    shader->UploadMat4("uProjection",
                       MainWindow::GetScene()->GetCamera()->GetProjMatrix());
    shader->UploadMat4("uView",
                       MainWindow::GetScene()->GetCamera()->GetViewMatrix());
    // 绑定vao
    glBindVertexArray(vaoID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 设置线宽
    glLineWidth(4);

    // 绘画
    glDrawArrays(GL_LINES, 0, lines.size() * 7 * 2);

    // 解绑
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

    shader->Detach();
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to) {
    AddLine2D(from, to, DEBUG_DEFAULT_COLOR, 1);
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec4 color) {
    AddLine2D(from, to, color, 1);
}

void DebugDraw::AddBox2D(glm::vec2 center, glm::vec2 dimensions,
                         float rotation) {
    AddBox2D(center, dimensions, rotation, DEBUG_DEFAULT_COLOR, 1);
}

void DebugDraw::AddBox2D(glm::vec2 center, glm::vec2 dimensions, float rotation,
                         glm::vec4 color) {
    AddBox2D(center, dimensions, rotation, color, 1);
}

void DebugDraw::AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec4 color,
                          int lifeTime) {
    if (lines.size() >= DEBUG_MAX_LINES) return;
    DebugDraw::lines.emplace_back(from, to, color, lifeTime);
}

void DebugDraw::AddBox2D(glm::vec2 center, glm::vec2 dimensions, float rotation,
                         glm::vec4 color, int lifetime) {
    auto mMin = center - dimensions;
    auto mMax = center + dimensions;

    glm::vec2 vertices[4] = {
        {mMin.x, mMin.y}, {mMin.x, mMax.y},
        {mMax.x, mMax.y}, {mMax.x, mMin.y},
    };

    // 旋转
    if (rotation != 0.f) {
        for (auto &v : vertices) {
            v = Rotate(v, rotation, center);
        }
    }

    AddLine2D(vertices[0], vertices[1], color, lifetime);
    AddLine2D(vertices[0], vertices[3], color, lifetime);
    AddLine2D(vertices[1], vertices[2], color, lifetime);
    AddLine2D(vertices[2], vertices[3], color, lifetime);
}
void DebugDraw::AddCircle(glm::vec2 center, float radius) {
    AddCircle(center, radius, DEBUG_DEFAULT_COLOR, 1);
}

void DebugDraw::AddCircle(glm::vec2 center, float radius, glm::vec4 color) {
    AddCircle(center, radius, color, 1);
}

void DebugDraw::AddCircle(glm::vec2 center, float radius, glm::vec4 color,
                          int lifetime) {
    int cnt = (int)(radius / 2);
    int mMax = 8;
    int lineCnt = cnt > mMax ? cnt : mMax;
    std::vector<glm::vec2> points;
    float increment = 360.0f / lineCnt;
    float currentAngle = 0;

    for (int i = 0; i < lineCnt; i++) {
        glm::vec2 tmp = {0, radius};
        tmp = Rotate(tmp, currentAngle);
        points.push_back(tmp + center);

        if (i > 0) {
            AddLine2D(points[i - 1], points[i], color, lifetime);
        }
        currentAngle += increment;
    }
    AddLine2D(points[lineCnt-1], points[0], color, lifetime);
}

glm::vec2 DebugDraw::Rotate(glm::vec2 pos, float rotation, glm::vec2 center) {
    auto mat = glm::translate(glm::mat4(1), {center, 0});
    mat = glm::rotate(mat, glm::radians(rotation), {0, 0, 1});
    mat = glm::translate(mat, {-center, 0});
    auto vv = mat * glm::vec4(pos.x, pos.y, 0, 1);
    return {vv.x, vv.y};
}
