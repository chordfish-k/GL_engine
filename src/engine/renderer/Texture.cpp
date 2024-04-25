#include "engine/util/Print.hpp"
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine/renderer/Texture.hpp"

Texture::Texture(std::string filePath) {
    this->filePath = filePath;

    // 让GPU生成纹理
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 设置纹理参数，在两个方向上repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 横向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 纵向
    // 不管方法还是缩小图片都显示像素(临近)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 使用stb库加载纹理
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    auto image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (image != nullptr) {
        if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, image);
        } else if (channels == 4) { // 有透明度
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, image);
        } else {
            util::Println("ERROR: (Texture) Unknown number of channels '",
                          channels, "'");
            assert(false);
        }
    } else {
        util::Println("ERROR: Could not load image '", filePath, "'");
        assert(false);
    }

    stbi_image_free(image);
}

Texture::~Texture() {}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}