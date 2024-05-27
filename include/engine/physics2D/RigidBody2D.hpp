#pragma once

#include "engine/node/Node.hpp"
#include "engine/physics2D/BodyType.hpp"
#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/Linear.h"
#include "engine/physics2D/Angular.h"
#include <box2d/box2d.h>

class RigidBody2D : public Node {
    COMPONENT(RigidBody2D)
    RTTR_ENABLE(Node)
protected:
    Linear linear;
    Angular angular;

    float mass = 1;
    BodyType bodyType = BodyType::Dynamic;

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

    json Serialize() override;

    RigidBody2D *Deserialize(json j) override;

    const glm::vec2 &GetLinearVelocity() const;

    void SetLinearVelocity(const glm::vec2 &velocity_);

    const float &GetAngularVelocity() const;

    void SetAngularVelocity(const float &velocity_);

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
