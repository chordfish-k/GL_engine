#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Camera.hpp"
#include "engine/core/AbstractScene.hpp"
#include "engine/core/GameObject.hpp"

#include "engine/component/SpriteRenderer.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Common.hpp"
#include "engine/util/Print.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

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
    LevelEditorScene() {}

    ~LevelEditorScene() {
        delete defaultShader;
        defaultShader = nullptr;
        delete camera;
        camera = nullptr;
    }

    void Init() {
        this->camera = new Camera();

        int xOffset = 10;
        int yOffset = 10;

        float totalWidth = (float)(600 - xOffset * 2);
        float totalHeight = (float)(300 - yOffset * 2);
        float sizeX = totalWidth / 100.0f;
        float sizeY = totalHeight / 100.0f;

        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                float xPos = xOffset + (x * sizeX);
                float yPos = yOffset + (y * sizeY);
                std::string goName = util::Concat("Obj", x, "-", y);
                GameObject *go = new GameObject(
                    goName, new Transform(glm::vec2(xPos, yPos),
                                          glm::vec2(sizeX, sizeY)));
                auto spr = new SpriteRenderer(
                    glm::vec4(xPos / totalWidth, yPos / totalHeight, 1, 1));

                go->AddComponent(spr);
                this->AddGameObject(go);
            }
        }

        InitResources();
    }

    void InitResources() { AssetPool::GetShader("assets/shader/default.glsl"); }

    void Update(float dt) {

        camera->position.x -= dt * 50.f;
        camera->position.y -= dt * 50.f;

        for (auto go : gameObjects) {
            go->Update(dt);
        }
        this->renderer->Render();
    }
};