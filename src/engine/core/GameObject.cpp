#include "engine/core/GameObject.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Transform.hpp"

#include <cassert>
#include <string>

GameObject::GameObject(std::string name)
    : GameObject(name, new Transform(), 0) {}

GameObject::GameObject(std::string name, Transform *transform)
    : GameObject(name, transform, 0) {}

GameObject::GameObject(std::string name, Transform *transform, int zIndex)
    : name(name), transform(transform), zIndex(zIndex) {
    this->components.push_back(transform);
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