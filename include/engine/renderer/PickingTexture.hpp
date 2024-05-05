#pragma once

class PickingTexture {
private:
    unsigned int pickingTextureID;
    unsigned int fboID;
    unsigned int depthTexture;

public:
    PickingTexture(int width, int height);

    void EnableWriting();

    void DisableWriting();

    int ReadPixel(int x, int y);

private:
    bool Init(int width, int height);
};
