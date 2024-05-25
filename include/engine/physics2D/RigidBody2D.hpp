#pragma once

#include "engine/node/Node.hpp"
#include "engine/physics2D/BodyType.hpp"
#include "engine/physics2D/ACollider.hpp"
#include <box2d/box2d.h>

class RigidBody2D : public Node {
    COMPONENT(RigidBody2D)
protected:
    glm::vec2 velocity = {0, 0};
    float angularDamping = 0.8f; // 角阻尼
    float linearDamping = 0.9f; // 线性阻尼
    float mass = 1;
    BodyType bodyType = Dynamic;

    bool fixedRotation = false;
    bool continuousCollision = true; // 连续碰撞，防止隧穿

    b2Body *rawBody = nullptr;
    ACollider *collider = nullptr;

public:
    RigidBody2D();

    ~RigidBody2D() override;

    void Start() override;

    void Update(float dt) override;

    void EditorUpdate(float dt) override;

    const glm::vec2 &GetVelocity() const;

    void SetVelocity(const glm::vec2 &velocity);

    float GetAngularDamping() const;

    void SetAngularDamping(float angularDamping_);

    float GetLinearDamping() const;

    void SetLinearDamping(float linearDamping);

    float GetMass() const;

    void SetMass(float mass);

    bool IsFixedRotation() const;

    void SetFixedRotation(bool fixedRotation);

    bool IsContinuousCollision() const;

    void SetContinuousCollision(bool continuousCollision);

    b2Body *GetRawBody() const;

    void SetRawBody(b2Body *rawBody);

    BodyType GetBodyType() const;

    void SetBodyType(BodyType bodyType);

    ACollider *GetCollider() const;

    void SetCollider(ACollider *collider);

    void Imgui() override;
};
