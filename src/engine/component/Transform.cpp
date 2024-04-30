#include "engine/component/Transform.hpp"
#include "engine/util/Print.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale) {
    this->position = position;
    this->scale = scale;
}

void Transform::Init() {}

void Transform::Start() {}

void Transform::Update(float dt) {}

Transform *Transform::Copy() const {
    return new Transform(glm::vec2(position), glm::vec2(scale));
}

void Transform::CopyTo(Transform *to) const {
    to->position = position;
    to->scale = scale;
}

void Transform::AddChild(Transform *tr) {
    if (tr->parent == this) return;
    tr->parent = this;
    children.push_back(tr);
}

void Transform::RemoveChild(Transform *transform) {
    auto &child = children;
    auto it = child.begin();
    for (int i = 0; i < child.size(); i++, it++) {
        if (child[i]->GetUid() == transform->GetUid()) {
            child.erase(it);
            break;
        }
    }

}


Transform &Transform::operator=(const Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    return *this;
}

glm::mat4 Transform::GetModelMatrix() {
    glm::mat4 modelMatrix = glm::mat4(1.0f); // 初始化为单位矩阵
    Transform * transform = this;
    // 递归计算父图形的综合变换并将其应用到模型矩阵上
    while (transform != nullptr) {
        glm::mat4 localTransform = glm::mat4(1.0f); // 本地变换矩阵，初始化为单位矩阵

        // 应用当前 Transform 的旋转、平移和缩放到局部变换矩阵上
        localTransform = glm::translate(localTransform, glm::vec3(transform->position.x, transform->position.y, 0));
        localTransform = glm::rotate(localTransform, glm::radians(transform->rotation), glm::vec3(0, 0, 1));
        localTransform = glm::scale(localTransform, glm::vec3(transform->scale.x, transform->scale.y, 1));
        // 将当前 Transform 的局部变换与模型矩阵相乘
        modelMatrix = localTransform * modelMatrix;

        // 获取父 Transform 并继续循环
        transform = transform->parent;
    }

    transform = this;



    return modelMatrix;
}

bool Transform::Equals(Transform &t) {
    if (&t == nullptr) return false;
    return t.position == this->position && t.scale == this->scale;
}

json Transform::Serialize() {
    json j;
    j["component"] = GetComponentName();
    j["uid"] = GetUid();
    j["position"] = {position.x, position.y};
    j["scale"] = {scale.x, scale.y};
    j["rotation"] = rotation;
    j["parent"] = parent ? parent->GetUid() : -1;
    for (int i = 0; i < children.size(); i++) {
        j["children"][i] = children[i]->GetUid();
    }
    return j;
}

Transform *Transform::Deserialize(json j) {
    Component::Deserialize(j);

    auto &p = j["position"];
    if (!p.empty())
        SetPosition(glm::vec2(p[0], p[1]));

    auto &s = j["scale"];
    if (!s.empty())
        SetScale(glm::vec2(s[0], s[1]));

    auto &r = j["rotation"];
    if (!r.empty())
        SetRotation(r);

    auto &ids = j["children"];
    if (!ids.empty())
        childrenUid.assign(ids.begin(), ids.end());

    auto &pid = j["parent"];
    if (!pid.empty())
        parentUid = pid;

    return this;
}

void Transform::Imgui() {
    Component::Imgui<Transform>();
}

