#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
class Line2D {
private:
    glm::vec2 from, to;
    glm::vec3 color;
    int lifeTime; // 线条将存在的帧数

public:
    Line2D(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime):
        from(from), to(to), color(color), lifeTime(lifeTime){}

    int BeginFrame();

    // 属性
    glm::vec2 GetFrom() {return from;}

    glm::vec2 GetTo() {return to;}

    glm::vec3 GetColor() {return color;}
};
