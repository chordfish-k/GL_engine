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
        delete c;
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

void GameObject::Imgui() {
    for (auto c : components) {
        c->Imgui();
    }
}

std::string GameObject::Serialize() {
    json j;
    j["name"] = name;
    j["zIndex"] = zIndex;
    for (int i = 0; i < components.size(); ++i) {
        j["components"][i] = Str2Json(components[i]->Serialize());
    }
    return j.dump(2);
}

ASerializableObj *GameObject::Deserialize(json j) {
    name = j["name"];
    zIndex = j["zIndex"];
    RemoveAllComponent();
    auto &comp = j["components"];
    for (auto &c : comp) {
        Component *component;
        if (c["component"] == "Transform") {
            component = new Transform();
            auto *cc = (Transform*)component;
            cc->Deserialize(c);
            cc->gameObject = this;
            transform = cc;
        } else if (c["component"] == "SpriteRenderer") {
            component = new SpriteRenderer();
            auto *cc = (SpriteRenderer*)component;
            cc->Deserialize(c);
            cc->gameObject = this;
        }
        components.push_back(component);
    }
    return this;
}

void GameObject::RemoveAllComponent() {
    for (auto c : components) {
        delete c;
    }
    components.clear();
}
