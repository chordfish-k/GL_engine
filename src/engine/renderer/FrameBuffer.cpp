#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "engine/renderer/FrameBuffer.hpp"

FrameBuffer::FrameBuffer(int width, int height) {
    // 生成帧缓冲
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // 创建用来渲染的目标纹理, 并附加到帧缓冲
    this->texture = new Texture(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->texture->GetId(), 0);

    // 创建渲染缓冲存储深度信息
    unsigned  int rboID;
    glGenRenderbuffers(1, &rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height); // 32位深度信息
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        assert(false && "ERROR: Framebuffer is not complete");
    }
    // 0表示解绑帧缓冲，也就是之后的数据渲染到窗口上
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetFboID() {
    return fboID;
}

Texture *FrameBuffer::GetTexture() {
    return texture;
}
