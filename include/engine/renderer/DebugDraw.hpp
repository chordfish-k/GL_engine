#pragma once

#include <vector>
#include "Line2D.hpp"
#include "Shader.hpp"

#define DEBUG_MAX_LINES 2000

class DebugDraw {
private:
    static std::vector<Line2D> lines;
    // 每个顶点6个浮点数，每条线两个顶点
    static std::vector<float> vertexArray;
    static Shader *shader;
    static unsigned int vaoID, vboID;
    static bool started;

public:
    static void Start();

    static void BeginFrame();

    static void Draw();

    static void AddLine2D(glm::vec2 from, glm::vec2 to);
    static void AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec4 color);
    static void AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec4 color, int lifeTime);

    static void AddBox2D(glm::vec2 center, glm::vec2 dimensions, float rotation);
    static void AddBox2D(glm::vec2 center, glm::vec2 dimensions, float rotation, glm::vec4 color);
    static void AddBox2D(glm::vec2 center, glm::vec2 dimensions, float rotation,
                         glm::vec4 color, int lifetime);

    static void AddCircle(glm::vec2 center, float radius);
    static void AddCircle(glm::vec2 center, float radius, glm::vec4 color);
    static void AddCircle(glm::vec2 center, float radius, glm::vec4 color, int lifetime);

private:
    static glm::vec2 Rotate(glm::vec2 pos, float rotation, glm::vec2 center = {0, 0});
};
