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
#include "engine/editor/PropertiesWindow.hpp"

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

        camera = new Camera(glm::vec2(-250, 0));
        sprites = AssetPool::GetSpritesheet("assets/image/spritesheets/decorationsAndBlocks.png");


        if (sceneLoaded) {
            if (!gameObjects.empty())
                PropertiesWindow::SetActiveGameObject(gameObjects[0]);
            return;
        }


        obj1 = new GameObject(
            "Object 1", new Transform(glm::vec2(200, 200), glm::vec2(256, 256)),
            1);
        auto *obj1SpriteRenderer = new SpriteRenderer();
        
        obj1SpriteRenderer->SetColor(glm::vec4(1, 0, 0, 1));

        obj1->AddComponent(obj1SpriteRenderer);
        AddGameObject(obj1);

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

        PropertiesWindow::SetActiveGameObject(obj1);
    }

    void InitResources() {
        AssetPool::GetShader("assets/shader/default.glsl");
        AssetPool::AddSpritesheet(
            "assets/image/spritesheet.png",
            new Spritesheet(
                AssetPool::GetTexture("assets/image/spritesheet.png"), 16, 16,
                26, 0));
        AssetPool::AddSpritesheet(
            "assets/image/spritesheets/decorationsAndBlocks.png",
            new Spritesheet(
                AssetPool::GetTexture(
                    "assets/image/spritesheets/decorationsAndBlocks.png"),
                16, 16, 81, 0));
    }

    void Update(float dt) override {
        for (auto go : gameObjects) {
            go->Update(dt);
        }
        this->renderer->Render();
    }

    void Imgui() override {
        ImGui::Begin("Test Window");

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;

        float windowX2 = windowPos.x + windowSize.x;


        for (int i = 0; i < sprites->Size(); i++) {
            Sprite *sprite = sprites->GetSprite(i);
            float  spriteWidth = sprite->GetWidth() * 2;
            float  spriteHeight = sprite->GetHeight() * 2;
            unsigned int id = sprite->GetTexId();
            std::vector<glm::vec2> texCoords = sprite->GetTexCoords();

            // imageButton默认以textureId作为id，也就是同一张纹理同一个id
            // 使用pushID和popID来给按钮特殊的id
            ImGui::PushID(i);
            if (ImGui::ImageButton((void*)(intptr_t)id, {spriteWidth, spriteHeight}, {texCoords[0].x, texCoords[0].y}, {texCoords[2].x, texCoords[2].y})) {
                util::Println("Button" , i , " clicked");
            }
            ImGui::PopID();

            ImVec2 lastButtonPos = ImGui::GetItemRectMax();
            float lastButtonX2 = lastButtonPos.x;
            float nextButtonX2 = lastButtonX2 + itemSpacing.x + spriteWidth;
            if (i + 1 < sprites->Size() && nextButtonX2 < windowX2) {
                ImGui::SameLine();
            }
        }
        ImGui::End();
    }
};