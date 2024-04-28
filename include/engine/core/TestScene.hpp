#pragma once

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/component/Sprite.hpp"
#include "engine/core/Camera.hpp"
#include "engine/core/AScene.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/component/Spritesheet.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/util/AssetPool.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

class TestScene : public AScene {
private:
    Spritesheet *sprites = nullptr;
    GameObject *obj1 = nullptr;

public:
    TestScene() = default;

    ~TestScene() = default;

    void Init() override {
        InitResources();

        this->camera = new Camera(glm::vec2(-250, 0));

        if (sceneLoaded) {
            if (!gameObjects.empty())
                activeGameObject = gameObjects[0];
            return;
        }

        // sprites = AssetPool::GetSpritesheet("assets/image/spritesheet.png");

        obj1 = new GameObject(
            "Object 1", new Transform(glm::vec2(200, 200), glm::vec2(256, 256)),
            1);
        auto *obj1SpriteRenderer = new SpriteRenderer();
        
        obj1SpriteRenderer->SetColor(glm::vec4(1, 0, 0, 1));

        obj1->AddComponent(obj1SpriteRenderer);
        AddGameObject(obj1);

        activeGameObject = obj1;

        auto *obj2 = new GameObject(
            "Object 2", new Transform(glm::vec2(400, 200), glm::vec2(256, 256)),
            0);
        auto *obj2SpriteRenderer = new SpriteRenderer();
        auto *obj2Sprite = new Sprite();
        obj2Sprite->SetTexture(
            AssetPool::GetTexture("assets/image/blendImage2.png"));
        obj2SpriteRenderer->SetSprite(obj2Sprite);

        obj2->AddComponent(obj2SpriteRenderer);
        AddGameObject(obj2);

        json j = obj2->Serialize();
        auto obj3 = new GameObject();
        obj3->Deserialize(j);
        assert(obj3 != nullptr);
        obj3->transform->position.y = 400;
        AddGameObject(obj3);

        activeGameObject = obj1;
    }

    void InitResources() {
        AssetPool::GetShader("assets/shader/default.glsl");
        AssetPool::AddSpritesheet(
            "assets/image/spritesheet.png",
            new Spritesheet(
                AssetPool::GetTexture("assets/image/spritesheet.png"), 16, 16,
                26, 0));
    }

    void Update(float dt) override {
        for (auto go : gameObjects) {
            go->Update(dt);
        }
        this->renderer->Render();
    }

    void Imgui() override {
        ImGui::Begin("Test Window");
        ImGui::Text("Some text");
        ImGui::End();
    }
};