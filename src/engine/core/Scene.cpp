﻿#pragma once

#include "engine/renderer/Renderer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/core/Camera.hpp"
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/editor/ImGuiFileDialog.h"
#include "engine/editor/FileDialog.hpp"
#include "engine/util/Common.hpp"

Scene::Scene(ASceneInitializer *sceneInitializer)
    :sceneInitializer(sceneInitializer) {
    renderer = new Renderer();
    luaScriptManager = new LuaScriptManager();
    physics2D = new Physics2D();
    root = new Node();
    sceneToolsRoot = new Node();
    sceneToolsRoot->SetDoSerialization(false);
    root->AddChildNode(sceneToolsRoot);
    root->name = "Root";
    isRunning = false;
}

Scene::~Scene() {
    delete root;
    delete renderer;
    delete physics2D;
    delete luaScriptManager;
    delete sceneInitializer;
    delete camera;
}

void Scene::Init() {
    camera = new Camera({0,0});
    sceneInitializer->LoadResources(this);
    sceneInitializer->Init(this);
}

void Scene::Start() {
    root->Start();
    isRunning = true;
    physics2D->Init();
}

void Scene::Update(float dt) {
    camera->AdjustProjection();
    luaScriptManager->OnGameUpdate(dt);
    physics2D->Update(dt);
    root->Update(dt);
    root->CheckDelete();
    if (root->ShouldDestroy()) delete root;
}

void Scene::EditorUpdate(float dt) {
    camera->AdjustProjection();
    root->EditorUpdate(dt);
    root->CheckDelete();
    if (root->ShouldDestroy()) delete root;
}

void Scene::AddNode(Node *n) const {
    root->AddChildNode(n);
    n->GeneratedId();
    if (isRunning) {
        n->Start();
    }
}

void Scene::AddNodeAsChild(Node *parent, Node *n) const {
    parent->AddChildNode(n);
    n->GeneratedId();
    if (isRunning) {
        n->Start();
    }
}

void Scene::RemoveAllNodes() const {
    root->RemoveAllNodes();
}

Camera *Scene::GetCamera() {
    return this->camera;
}

void Scene::SceneImgui() {
    Imgui();
}

void Scene::Imgui() {
    sceneInitializer->Imgui();
}

json Scene::Serialize() const {
    json j;
    j = root->Serialize();
    return j;
}

Node *Scene::Deserialize(json j) const {
    RemoveAllNodes();
    root->Deserialize(j);
    return root;
}

void Scene::Save() {
    auto path = sceneInitializer->GetFilePath();

    auto save = [&](const std::string &path) {
        util::Println("Save:", path);

        std::string jsonText = util::Dump(this->Serialize());// this->Serialize().dump(2);
        std::ofstream out(path, std::ios::trunc);
        if (out.is_open()){
            out << jsonText;
            out.close();
        } else {
            util::Println("ERROR:Saving Scene failed.");
        }
    };

    if (path.empty()) {
        FileDialog::DisplayWithCallback("save",
                                        [=](auto filePath, auto name){
            sceneInitializer->SetFilePath(name);
            save(name);
        }, "scene.scene", ".scene");
    } else {
        save(path);
    }
}

void Scene::Load() {
    auto path = sceneInitializer->GetFilePath();
    if (path == "") return;

    util::Println("Load:", path);
    root->Deserialize(util::LoadJsonFromFile(path));
}

void Scene::Destroy() {
    root->Destroy();
    shouldDestroy = true;
}

Renderer *Scene::GetRenderer() const {
    return renderer;
}

Physics2D *Scene::GetPhysics2D() const {
    return physics2D;
}

LuaScriptManager *Scene::GetLuaScriptManager() const {
    return luaScriptManager;
}

void Scene::Render() {
    this->renderer->Render();
}

Node *Scene::GetNodeByUid(int uid) {
    Node *node = nullptr;
    root->TravelOnSubTree([&](auto n){
        if (n->GetUid() == uid) {
            node = n;
            return;
        }
    });
    return node;
}

ASceneInitializer *Scene::GetSceneInitializer() const {
    return sceneInitializer;
}

bool Scene::IsRunning() const {
    return isRunning;
}
