

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/renderer/Shader.hpp"
#include "engine/util/Common.hpp"
#include "engine/util/Print.hpp"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

Shader::Shader(const char *filePath) {
    // 1. 读取着色器文件
    std::string source;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        source = shaderStream.str();
    } catch (std::ifstream::failure &e) {
        util::Println("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: ", e.what());
    }
    const char *sourceCode = source.c_str();

    // 2.分割成顶点着色器和片段着色器
    // std::string text = "#type vertice";
    std::regex pattern("#type ([a-zA-Z]+)"); // 注意：\\b表示单词边界
    std::pmr::vector<std::smatch> matches;

    // 在字符串中查找所有匹配项
    // 遍历所有匹配项
    for (std::sregex_iterator it(source.begin(), source.end(), pattern);
         it != std::sregex_iterator(); ++it) {
        std::smatch match = *it;
        matches.push_back(match);
    }
    size_t fShaderBegin = matches[1].position() + matches[1].size();
    if (matches.size() != 2) {
        util::Println("ERROR::SHADER::GLSL_PARESING_FAILED: vertex or "
                      "fragment shader declaration missed.");
    }

    size_t index = source.find("#type") + 6;
    size_t eol = source.find("\n", index);
    std::string firstPattern = util::Trim(source.substr(index, eol - index));

    size_t index2 = source.find("#type", eol) + 6;
    size_t eol2 = source.find("\n", index2);
    std::string secondPattern =
        util::Trim(source.substr(index2, eol2 - index2));

    size_t piece1from = eol + 1;
    size_t piece2from = eol2 + 1;
    size_t piece1n = (index2 - 6) - (piece1from);
    size_t piece2n = (source.size()) - (piece2from);

    if (firstPattern == "vertex") {
        vertexSource = util::Trim(source.substr(piece1from, piece1n)).c_str();
    } else if (firstPattern == "fragment") {
        fragmentSource = util::Trim(source.substr(piece1from, piece1n)).c_str();
    } else {
        util::Println("Unexpect token '", firstPattern, "'");
    }

    if (secondPattern == "vertex") {
        vertexSource = util::Trim(source.substr(piece2from, piece2n)).c_str();
    } else if (secondPattern == "fragment") {
        fragmentSource = util::Trim(source.substr(piece2from, piece2n)).c_str();
    } else {
        util::Println("Unexpect token '", firstPattern, "'");
    }
}

Shader::~Shader() {
    detach();
}

// 编译着色器
void Shader::compile() {
    // *************
    // 编译并连接着色器
    // *************

    // 顶点着色器
    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexSource, NULL);
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
    glShaderSource(fragmentID, 1, &fragmentSource, NULL);
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

    delete[] vertexSource;
    delete[] fragmentSource;
}

// 绑定着色器程序
void Shader::use() {
    glUseProgram(shaderProgramID);
}

// 解绑
void Shader::detach() {
    glUseProgram(0);
}

void Shader::UploadMat4(const char *varName, glm::mat4 mat4) {
    int varLocation = glGetUniformLocation(shaderProgramID, varName);
    glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat4));
}