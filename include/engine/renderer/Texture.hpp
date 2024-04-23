#pragma once

class Texture {
private:
    const char *filePath;
    unsigned int texID;

public:
    Texture(const char *filePath);
    ~Texture();

    void Bind();

    void Unbind();
};