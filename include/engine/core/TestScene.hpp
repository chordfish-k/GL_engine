#pragma once

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Camera.hpp"
#include "engine/core/AScene.hpp"
#include "engine/renderer/Spritesheet.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/node/EditorMouseControls.hpp"
#include "Prefabs.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/node/EditorGridLines.hpp"
#include "engine/node/EditorCameraControls.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

class TestScene : public AScene {
private:
    Spritesheet *sprites = nullptr;
    EditorMouseControls *mouseControls = nullptr;
    EditorGridLines *gridLines = nullptr;
    EditorCameraControls *cameraControls = nullptr;
public:
    TestScene() {
        mouseControls = sceneToolsRoot->AddNode<EditorMouseControls>();
        gridLines = sceneToolsRoot->AddNode<EditorGridLines>();
        cameraControls = sceneToolsRoot->AddNode<EditorCameraControls>();
    }

    ~TestScene() = default;

    void Init() override {
        InitResources();

        camera = new Camera(glm::vec2(0,0));
        cameraControls->SetEditorCamera(camera);

        sprites = AssetPool::GetSpritesheet("assets/image/spritesheets/decorationsAndBlocks.png");

        if (sceneLoaded) {
            if (!Window::GetScene()->root->children.empty())
//                PropertiesWindow::SetActiveNode(Window::GetScene()->root->children[0]);
            return;
        }

        auto node = root->AddNode<Node>("Group")
            ->SetTransform({glm::vec2(0, 0), glm::vec2(5, 5)});
        auto obj1 = node->AddNode<SpriteRenderer>("SpriteRenderer 1")
            ->SetColor({1, 1, 1, 1})
                        ->SetSprite(sprites->GetSprite(0))
                        ->SetTransform({glm::vec2(0, 0), glm::vec2(1, 1)});
        node->AddNode<SpriteRenderer>("SpriteRenderer 2")
            ->SetColor({1, 1, 1, 1})
                        ->SetSprite(sprites->GetSprite(1))
                        ->SetTransform({glm::vec2(16, 0), glm::vec2(1, 1)});
        obj1->AddNode<SpriteRenderer>("SpriteRenderer 3")
            ->SetColor({1, 1, 1, 1})
                        ->SetSprite(sprites->GetSprite(2))
                        ->SetTransform({glm::vec2(0, 16), glm::vec2(1, 1)});
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
        static float angle = 0;
        DebugDraw::AddBox2D({400, 200}, {64, 32}, angle+=10.0f*dt, {0, 1, 0}, 1);
        DebugDraw::AddCircle({600, 200}, 128);

        AScene::Update(dt);
        Render();
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
//                util::Println("Button" , i , " clicked");
                Node *node = Prefabs::GenerateSpriteNode(sprite, {1, 1});
                mouseControls->PickupNode(node);
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