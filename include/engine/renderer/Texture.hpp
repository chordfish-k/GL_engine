#pragma once

#include <string>
#include <glm/vec2.hpp>
class Texture {
private:
    std::string filePath;
    unsigned int texID;
    int width, height;

public:
    Texture() :texID(-1), width(-1), height(-1){};

    Texture(int width, int height);

    ~Texture();

    bool Init(std::string filePath);

    void Bind();

    void Unbind();

    // 属性
    int GetWidth() { return width; }

    int GetHeight() { return height; }

    glm::vec2 GetSize() {return {width, height};}

    const std::string &GetFilePath() const { return filePath; }

    unsigned int GetId() const { return texID; }
};