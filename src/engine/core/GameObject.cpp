#include "engine/core/GameObject.hpp"
#include "engine/component/SpriteRenderer.hpp"
#include "engine/component/Transform.hpp"
#include "engine/core/Window.hpp"

#include <cassert>
#include <string>

int GameObject::ID_COUNTER = 0;

GameObject::GameObject(std::string name)
    : GameObject(name, new Transform(), 0) {}

GameObject::GameObject(std::string name, Transform *transform)
    : GameObject(name, transform, 0) {}

GameObject::GameObject(std::string name, Transform *transform, int zIndex)
    : name(name), transform(transform){
    AddComponent(transform);
    Window::GetScene()->root->AddChild(transform);
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

json GameObject::Serialize() {
    json j;
    j["name"] = name;
    for (int i = 0; i < components.size(); ++i) {
        j["components"][i] = components[i]->Serialize();
    }
    return j;
}

ASerializableObj *GameObject::Deserialize(json j) {
    name = j["name"];
    RemoveAllComponent();
    auto &comp = j["components"];
    for (auto &c : comp) {
        Component *component;
        if (c["component"] == "Transform") {
            component = new Transform();
            auto *cc = (Transform*)component;
            cc->Deserialize(c)->gameObject = this;
            transform = cc;
        } else if (c["component"] == "SpriteRenderer") {
            component = new SpriteRenderer();
            auto *cc = (SpriteRenderer*)component;
            cc->Deserialize(c)->gameObject = this;
        }
        AddComponent(component);
    }
    return this;
}

void GameObject::RemoveAllComponent() {
    for (auto c : components) {
        delete c;
    }
    components.clear();
}

Component *GameObject::AddComponent(Component *comp) {
    components.push_back(comp);
    comp->gameObject = this;
    comp->GeneratedId();
    return comp;
}


int GameObject::GetUid() const {
    return uid;
}

bool GameObject::IsDead() const {
    return isDead;
}

bool GameObject::IsDoSerialization() const {
    return doSerialization;
}
void GameObject::SetNoSerialization() {
    GameObject::doSerialization = false;
}

void GameObject::GenerateUid() {
    this->uid = ID_COUNTER++;
}
void GameObject::Destroy() {
    this->isDead = true;
    for (auto c : components) {
        c->Destroy();
    }
}

void GameObject::init(int maxId) {
    // 加载一个go后，没有go会获得与之相同的id
    ID_COUNTER = maxId;
}

void GameObject::AddChild(GameObject *ch) {
    if (ch == this) return;

    if (ch->transform->parent)
        ch->transform->parent->RemoveChild(ch->transform);

    auto &child = transform->children;
    auto it = std::find(child.begin(), child.end(), ch->transform);
    if (it == child.end()) {
        transform->AddChild(ch->transform);
    }
}

void GameObject::RemoveChild(GameObject *ch) {
    auto &child = ch->transform->children;
    auto it = std::find(child.begin(), child.end(), ch->transform);
    if (it != child.end()) {
        child.erase(it);
    }
}

std::vector<Component *> &GameObject::GetComponents() {
    return components;
}
