#pragma once

#include "AbstractScene.hpp"
#include "Window.hpp"
#include "engine/core/Window.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/util/Print.hpp"
#include "engine/util/Common.hpp"

class LevelEditorScene : public AbstractScene {
private:
    Shader *defaultShader;

    unsigned int shaderProgram;
    unsigned int vboID, vaoID, eboID;

    float verticesArray[28] = {
        // position                 // color
        0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上 2
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 右下 0
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 左下 3
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 左上 1

    };
    unsigned int elementArray[6] = {
        0, 1, 3, 1, 2, 3,
    };

public:
    LevelEditorScene() { util::Println("Inside level editor scene."); }

    ~LevelEditorScene() {
        delete defaultShader;
    }

    void Init() {
        defaultShader = new Shader(
            "assets/shader/default.glsl");
        defaultShader->compile();

        // ********************************
        // 创建VAO, VBO, EBO 缓冲， 并发送到GPU
        // ********************************

        // 生成并绑定VAO
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        // 创建VBO
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), verticesArray,
                     GL_STATIC_DRAW);

        // 创建EBO
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray),
                     elementArray, GL_STATIC_DRAW);

        // 设置顶点属性指针
        int positionSize = 3;
        int colorSize = 4;
        int floatSizeBytes = sizeof(float);
        int vertexSizeBytes = (positionSize + colorSize) * floatSizeBytes;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *)(positionSize * floatSizeBytes));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Update(float dt) {
        // 绑定着色器程序
        defaultShader->use();

        // 绑定要用的VAO
        glBindVertexArray(vaoID);

        // 启用顶点属性指针
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // 根据ebo画三角形
        glDrawElements(GL_TRIANGLES, util::LenOf(elementArray), GL_UNSIGNED_INT,
                       0);

        // 解绑
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
        defaultShader->detach();
    }
};