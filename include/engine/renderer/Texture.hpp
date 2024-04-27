#pragma once

#include <string>
class Texture {
private:
    std::string filePath;
    unsigned int texID;
    int width, height;

public:
    Texture() {};

    ~Texture();

    void Init(std::string filePath);

    void Bind();

    void Unbind();

    // 属性
    int GetWidth() { return width; }

    int GetHeight() { return height; }

    const std::string &GetFilePath() const;

};