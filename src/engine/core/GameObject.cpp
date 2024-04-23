#include "engine/core/GameObject.hpp"
#include "engine/component/SpriteRenderer.hpp"

#include <cassert>
#include <string>
#include <type_traits>

GameObject::GameObject(std::string name) {
    SpriteRenderer *sr = new SpriteRenderer();
    this->GetComponent<SpriteRenderer>();
}

GameObject::~GameObject() {
    for (auto c : components) {
        if (c == nullptr)
            continue;
        delete c;
        c = nullptr;
    }
}

void GameObject::Update(float dt) {
    for (auto c : components) {
        c->Update(dt);
    }
}

void GameObject::Start() {
    for (auto c : components) {
        c->Start();
    }
}