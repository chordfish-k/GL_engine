#pragma once

#include "engine/renderer/Texture.hpp"

class FrameBuffer {
protected:
    unsigned int fboID = 0;
private:
    Texture *texture = nullptr;

public:
    FrameBuffer(int width, int height);

    void Bind();

    void Unbind();

    unsigned int GetFboID();

    Texture *GetTexture();
};
