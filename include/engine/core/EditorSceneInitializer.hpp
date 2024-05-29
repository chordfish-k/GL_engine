#pragma once

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Camera.hpp"
#include "engine/core/Scene.hpp"
#include "engine/renderer/Spritesheet.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/node/EditorMouseControls.hpp"
#include "engine/util/PrefabsUtils.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/node/EditorGridLines.hpp"
#include "engine/node/EditorCameraControls.hpp"
#include "engine/node/EditorSpriteGizmo.hpp"
#include "engine/core/ASceneInitializer.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

class EditorSceneInitializer : public ASceneInitializer {
private:
    Spritesheet *sprites = nullptr;
    EditorMouseControls *mouseControls = nullptr;
    EditorGridLines *gridLines = nullptr;
    EditorCameraControls *cameraControls = nullptr;
    EditorSpriteGizmo *spriteGizmo = nullptr;

public:
    EditorSceneInitializer(const std::string &filePath = "")
        : ASceneInitializer(filePath){};

    ~EditorSceneInitializer() {

    }

    void Init(Scene *scene) override {
        sprites = AssetPool::GetSpritesheet("assets/image/spritesheets/decorationsAndBlocks.png");

        auto r = scene->sceneToolsRoot;
        mouseControls = r->AddNode<EditorMouseControls>();
        gridLines = r->AddNode<EditorGridLines>();
        cameraControls = r->AddNode<EditorCameraControls>();
        cameraControls->SetEditorCamera(scene->GetCamera());
        spriteGizmo = r->AddNode<EditorSpriteGizmo>();
    }

    void LoadResources(Scene *scene) override {
        AssetPool::AddSpritesheet(
                    "assets/image/spritesheets/decorationsAndBlocks.png",
                16, 16, 81);
    }

    void Imgui() override {
        // TODO spritesheet中sprite对象缓存
        /*
        ImGui::Begin("Spritesheet");

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
//                Node *node = PrefabsUtils::GenerateSpriteNode(sprite, {1, 1});
                Node *node = PrefabsUtils::GenerateSpriteRendererFromSpriteSheet(sprites, i);
                mouseControls->PickupNode(node);
            }
            ImGui::PopID();

            ImVec2 lastButtonPos = ImGui::GetItemRectMax();
            float lastButtonX2 = lastButtonPos.x;
            float nextButtonX2 = lastButtonX2 + itemSpacing.x + spriteWidth;
            if (i + 1 < sprites->Size() && nextButtonX2 < windowX2) {
                ImGui::SameLine();
            }
            delete sprite;
        }
        ImGui::End();
         */
    }
};