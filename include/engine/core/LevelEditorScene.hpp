#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/AbstractScene.hpp"
#include "engine/core/Camera.hpp"
#include "engine/core/GameObject.hpp"

#include "engine/component/SpriteRenderer.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include "engine/util/Print.hpp"
#include "engine/util/Common.hpp"
#include "engine/util/Time.hpp"

#include <glm/ext/vector_float2.hpp>

class LevelEditorScene : public AbstractScene {
private:
    Shader *defaultShader;
    Texture *testTexture;
    GameObject *testObj;
    bool firstTime = false;

    unsigned int shaderProgram;
    unsigned int vboID, vaoID, eboID;

    float verticesArray[36] = {
        // position                 // color // UV
        100.5f,  100.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1, 0, // 右上 0
        100.5f,  -100.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1, 1, // 右下 1
        -100.5f, -100.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0, 1, // 左下 2
        -100.5f, 100.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0, 0, // 左上 3

    };
    unsigned int elementArray[6] = {
        0, 1, 3, 1, 2, 3,
    };

public:
    LevelEditorScene() { util::Println("Inside level editor scene."); }

    ~LevelEditorScene() {
        delete defaultShader;
        defaultShader = nullptr;
        delete camera;
        camera = nullptr;
    }

    void Init() {
        // 初始化一个GameObject
        testObj = new GameObject("test object");
        testObj->AddComponent<SpriteRenderer>();
        AddGameObject(testObj);

        // 初始化摄像机
        this->camera = new Camera(glm::vec2(0.f, 0.f));

        // 加载Shader
        defaultShader = new Shader("assets/shader/default.glsl");
        defaultShader->Compile();

        // 加载纹理
        testTexture = new Texture("assets/image/testImage.png");

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
        int uvSize = 2;
        int floatSizeBytes = sizeof(float);
        int vertexSizeBytes =
            (positionSize + colorSize + uvSize) * floatSizeBytes;

        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE,
                              vertexSizeBytes, (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *)(positionSize * floatSizeBytes));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2, uvSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
            (void *)((positionSize + colorSize) * floatSizeBytes));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        camera->position.x = -500.f;
        camera->position.y = -300.f;
    }

    void Update(float dt) {

        // 绑定着色器程序
        defaultShader->Use();

        // 上传纹理到着色器
        defaultShader->UploadTexture("TEX_SAMPLER", 0);
        glActiveTexture(GL_TEXTURE0);
        testTexture->Bind();

        // 上传坐标变换矩阵到着色器
        defaultShader->UploadMat4("uProjection", camera->GetProjMatrix());
        defaultShader->UploadMat4("uView", camera->GetViewMatrix());
        defaultShader->UploadFloat("uTime", Time::GetGameTime());

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
        defaultShader->Detach();

        for (GameObject *go : gameObjects) {
            go->Update(dt);
        }
    }
};