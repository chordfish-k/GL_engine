#pragma once

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Camera.hpp"
#include "engine/core/AbstractScene.hpp"
#include "engine/core/GameObject.hpp"

#include "engine/component/SpriteRenderer.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

class TestScene : public AbstractScene {
public:
    TestScene() {}

    ~TestScene() {}

    void Init() {
        this->camera = new Camera();

        GameObject *obj1 =
            new GameObject("Object 1", new Transform(glm::vec2(100, 200),
                                                     glm::vec2(256, 256)));
        obj1->AddComponent(new SpriteRenderer(
            AssetPool::GetTexture("assets/image/testImage.png")));
        this->AddGameObject(obj1);

        GameObject *obj2 =
            new GameObject("Object 2", new Transform(glm::vec2(400, 200),
                                                     glm::vec2(256, 256)));
        obj2->AddComponent(new SpriteRenderer(
            AssetPool::GetTexture("assets/image/testImage2.png")));
        this->AddGameObject(obj2);

        InitResources();
    }

    void InitResources() { AssetPool::GetShader("assets/shader/default.glsl"); }

    void Update(float dt) {
        for (auto go : gameObjects) {
            go->Update(dt);
        }
        this->renderer->Render();
    }
};