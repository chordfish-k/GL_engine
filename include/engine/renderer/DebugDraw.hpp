#pragma once

#include <vector>
#include "Line2D.hpp"
#include "Shader.hpp"

#define DEBUG_MAX_LINES 500

class DebugDraw {
private:
    static std::vector<Line2D> lines;
    // 每个顶点6个浮点数，每条线两个顶点
    static float vertexArray[DEBUG_MAX_LINES * 6 * 2];
    static Shader *shader;
    static unsigned int vaoID, vboID;
    static bool started;

public:
    static void Start();

    static void BeginFrame();

    static void Draw();

    static void AddLine2D(glm::vec2 from, glm::vec2 to);
    static void AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec3 color);
    static void AddLine2D(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime);
};
