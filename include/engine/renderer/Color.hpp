#pragma once

#include <glm/vec4.hpp>

struct Color {
    float r, g, b, a;
    Color(float r, float g, float b, float a)
        :r(r), g(g), b(b), a(a){}
    friend bool operator==(const Color &c1, const Color &c2){
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
    }
    glm::vec4 ToVec4() const{
        return {r, g, b, a};
    }
};
