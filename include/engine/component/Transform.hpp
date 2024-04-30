#pragma once

#include "Component.hpp"
#include "engine/component/Component.hpp"
#include <glm/ext/vector_float2.hpp>

COMPONENT(Transform)
public:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation = 0.f;
    Transform *parent = nullptr;
    std::vector<Transform *> children;

public:
    std::vector<int> childrenUid;
    int parentUid = -1;

public:
    Transform() : Transform(glm::vec2(), glm::vec2(1, 1)) {}

    Transform(glm::vec2 position) : Transform(position, glm::vec2(1, 1)) {}

    Transform(glm::vec2 position, glm::vec2 scale);

    void Init();

    void Start() override;

    void Update(float dt) override;

    Transform *Copy() const;

    void CopyTo(Transform *to) const;

    Transform &operator=(const Transform &t);

    bool Equals(Transform &t);

    json Serialize() override;

    Transform *Deserialize(json j) override;

    void AddChild(Transform *tr);


    void Imgui();

    // 属性
    glm::vec2 GetPosition() {
        return position;
    }

    glm::vec2 GetAbsPosition() {
        return parent ? parent->GetAbsPosition() + position : position;
    }

    void SetPosition(const glm::vec2 pos) {
        this->position = pos;
    }

    glm::vec2 GetScale() {
        return scale;
    }

    glm::vec2 GetAbsScale() {
        return parent ? parent->GetAbsScale() * scale : scale;
    }

    void SetScale(const glm::vec2 scale) {
        this->scale = scale;
    }

    float GetRotation() {
        return rotation;
    }

    float GetAbsRotation() {
        return parent ? parent->GetAbsRotation() + rotation : rotation;
    }

    void SetRotation(float rotation) {
        this->rotation = rotation;
    }

    glm::mat4 GetModelMatrix();

    void RemoveChild(Transform *pTransform);
    };
