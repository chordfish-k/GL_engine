#pragma once

#include <string>
class Texture {
private:
    std::string filePath;
    unsigned int texID;

public:
    Texture(std::string filePath);
    ~Texture();

    void Bind();

    void Unbind();
};