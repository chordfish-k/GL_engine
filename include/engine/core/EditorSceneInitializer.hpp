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
//        sprites = AssetPool::GetSpritesheet("assets/image/spritesheets/decorationsAndBlocks.png");

        auto r = scene->sceneToolsRoot;
        mouseControls = r->AddNode<EditorMouseControls>();
        gridLines = r->AddNode<EditorGridLines>();
        cameraControls = r->AddNode<EditorCameraControls>();
        cameraControls->SetEditorCamera(scene->GetCamera());
        spriteGizmo = r->AddNode<EditorSpriteGizmo>();
    }

    void LoadResources(Scene *scene) override {
//        AssetPool::AddSpritesheet(
//                    "assets/image/spritesheets/decorationsAndBlocks.png",
//                16, 16, 81);
    }

    void Imgui() override {

    }
};