#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
private:
    std::string filePath;

    unsigned int shaderProgramID;
    bool beingUsed;

    std::string vertexSource;
    std::string fragmentSource;

public:
    Shader(std::string filePath);

    ~Shader();

    // 编译着色器
    void Compile();

    // 绑定着色器程序
    void Use();

    // 解绑
    void Detach();

    // 数据上传
    void UploadMat4(const char *varName, glm::mat4 mat4);
    void UploadMat3(const char *varName, glm::mat3 mat3);
    void UploadVec4(const char *varName, glm::vec4 vec4);
    void UploadVec3(const char *varName, glm::vec3 vec3);
    void UploadInt(const char *varName, int slot);
    void UploadFloat(const char *varName, float slot);
    void UploadTexture(const char *varName, int slot);
    void UploadIntArray(const char *varName, int size, int *array);
};