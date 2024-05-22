#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/renderer/PickingTexture.hpp"

PickingTexture::PickingTexture(int width, int height) {
    if (!Init(width, height)) {
        assert(false && "ERROR: Failed to init picking texture.");
    }
}

void PickingTexture::EnableWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
}

void PickingTexture::DisableWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

int PickingTexture::ReadPixel(int x, int y) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    float pixels[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);

    return (int)pixels[0]-1;
}

bool PickingTexture::Init(int width, int height) {
    // 生成帧缓冲
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // 创建用来渲染的目标纹理, 并附加到帧缓冲
    glGenTextures(1, &pickingTextureID);
    glBindTexture(GL_TEXTURE_2D, pickingTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0,
                 GL_RGB, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           pickingTextureID, 0);

    // 创建用于深度缓冲的材质对象
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           depthTexture, 0);

    // 禁用Framebuffer的读取
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    // 解绑帧缓冲和材质
    glBindTexture(GL_TEXTURE_2D, 0);
    // 0表示解绑帧缓冲，也就是之后的数据渲染到窗口上
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}
