#pragma once

#include <string>
class Texture {
private:
    std::string filePath;
    unsigned int texID;
    int width, height;

public:
    Texture(std::string filePath);

    ~Texture();

    void Bind();

    void Unbind();

    // 属性
    int GetWidth() { return width; }

    int GetHeight() { return height; }
};