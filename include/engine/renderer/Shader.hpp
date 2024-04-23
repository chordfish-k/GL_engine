#pragma once

#include <glm/ext/matrix_float4x4.hpp>

class Shader {
private:
    unsigned int shaderProgramID;
    const char *vertexSource;
    const char *fragmentSource;

public:
    Shader(const char *filePath);

    ~Shader();

    // 编译着色器
    void compile();

    // 绑定着色器程序
    void use();

    // 解绑
    void detach();

    // 数据上传
    void UploadMat4(const char*varName, glm::mat4 mat4);
};