#include "engine/core/GameObject.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Transform.hpp"

#include <cassert>
#include <string>

GameObject::GameObject(std::string name) {
    this->name = name;
    this->transform = new Transform();
    this->components.push_back(this->transform);
}

GameObject::GameObject(std::string name, Transform *transform) {
    this->name = name;
    this->transform = transform;
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