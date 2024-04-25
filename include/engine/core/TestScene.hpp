#pragma once

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Camera.hpp"
#include "engine/core/AbstractScene.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/component/Spritesheet.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/util/AssetPool.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

class TestScene : public AbstractScene {
private:
    Spritesheet *sprites;
    GameObject *obj1;

public:
    TestScene() {}

    ~TestScene() {}

    void Init() {
        InitResources();

        this->camera = new Camera(glm::vec2(-250, 0));

        sprites = AssetPool::GetSpritesheet("assets/image/spritesheet.png");

        obj1 = new GameObject("Object 1", new Transform(glm::vec2(100, 200),
                                                        glm::vec2(256, 256)));
        obj1->AddComponent(new SpriteRenderer(sprites->GetSprite(0)));
        this->AddGameObject(obj1);

        GameObject *obj2 =
            new GameObject("Object 2", new Transform(glm::vec2(400, 200),
                                                     glm::vec2(256, 256)));
        obj2->AddComponent(new SpriteRenderer(sprites->GetSprite(15)));
        this->AddGameObject(obj2);
    }

    void InitResources() {
        AssetPool::GetShader("assets/shader/default.glsl");
        AssetPool::AddSpritesheet(
            "assets/image/spritesheet.png",
            new Spritesheet(
                AssetPool::GetTexture("assets/image/spritesheet.png"), 16, 16,
                26, 0));
    }

    void Update(float dt) {
        spriteFlipTimeLeft -= dt;
        if (spriteFlipTimeLeft <= 0) {
            spriteFlipTimeLeft = spriteFlipTime;
            spriteIndex++;
            if (spriteIndex >= 4) {
                spriteIndex = 0;
            }
            obj1->GetComponent<SpriteRenderer>()->SetSprite(
                sprites->GetSprite(spriteIndex));
        }

        for (auto go : gameObjects) {
            go->Update(dt);
        }
        this->renderer->Render();
    }

private:
    int spriteIndex = 0;
    float spriteFlipTime = 0.2f;
    float spriteFlipTimeLeft = 0.0f;
};