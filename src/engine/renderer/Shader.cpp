
#include "engine/renderer/Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/util/Common.hpp"
#include "engine/util/Print.hpp"

#include <cstddef>
#include <cstring>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

Shader::Shader(std::string filePath) {
    this->filePath = filePath;
    this->beingUsed = false;

    // 1. 读取着色器文件
    std::string source;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath.c_str());

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        source = shaderStream.str();
    } catch (std::ifstream::failure &e) {
        util::Println("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: ", e.what());
    }

    // 2.分割成顶点着色器和片段着色器
    std::regex pattern("#type (\\w+)\\n([\\s\\S]*?)(?=#type |$)");
    std::sregex_iterator iter(source.begin(), source.end(), pattern);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string type = util::Trim(match.str(1));
        std::string content = util::Trim(match.str(2));

        if (type == "vertex") {
            vertexSource = content;
        } else if (type == "fragment") {
            fragmentSource = content;
        } else {
            util::Println("Unexpect token '", type, "'");
        }
    }
}

Shader::~Shader() {
    if (beingUsed)
        Detach();
}

// 编译着色器
void Shader::Compile() {
    // *************
    // 编译并连接着色器
    // *************

    // 顶点着色器
    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    const char *vSource = vertexSource.c_str();
    glShaderSource(vertexID, 1, &vSource, NULL);
    glCompileShader(vertexID);
    // 错误检查
    int success;
    char infoLog[512];
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
        util::Println("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog);
    }
    // 片段着色器
    unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fSource = fragmentSource.c_str();
    glShaderSource(fragmentID, 1, &fSource, NULL);
    glCompileShader(fragmentID);
    // 错误检查
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
        util::Println("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
    }
    // 连接着色器
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexID);
    glAttachShader(shaderProgramID, fragmentID);
    glLinkProgram(shaderProgramID);
    // 检查连接错误
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

// 绑定着色器程序
void Shader::Use() {
    if (!this->beingUsed)
        glUseProgram(shaderProgramID);
}

// 解绑
void Shader::Detach() {
    glUseProgram(0);
    beingUsed = false;
}

void Shader::UploadMat4(const char *varName, glm::mat4 mat4) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::UploadMat3(const char *varName, glm::mat3 mat3) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniformMatrix3fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat3));
}

void Shader::UploadVec4(const char *varName, glm::vec4 vec4) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform4f(varLocation, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::UploadVec3(const char *varName, glm::vec3 vec3) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform3f(varLocation, vec3.x, vec3.y, vec3.z);
}

void Shader::UploadFloat(const char *varName, float val) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform1f(varLocation, val);
}

void Shader::UploadInt(const char *varName, int val) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform1i(varLocation, val);
}

void Shader::UploadTexture(const char *varName, int slot) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform1i(varLocation, slot);
}

void Shader::UploadIntArray(const char *varName, int size, int *array) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    Use();
    glUniform1iv(varLocation, size, array);
}