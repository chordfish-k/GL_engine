#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/physics2D/Box2DCollider.hpp"

RigidBody2D::RigidBody2D() {
    collider = new Box2DCollider();
    auto c = (Box2DCollider*) collider;
    c->SetHalfSize({40, 40});
    mass = 10;

}

RigidBody2D::~RigidBody2D() {
    delete collider;
}

void RigidBody2D::Update(float dt)  {
    if (rawBody != nullptr) {
        // 更新rawBody的实际位置到gameObject的transform组件
        transform.position = {rawBody->GetPosition().x,
                              rawBody->GetPosition().y};
        transform.rotation = (float) glm::degrees(rawBody->GetAngle());
        auto vel = rawBody->GetLinearVelocity();
        velocity = {vel.x, vel.y};
    }

    if (collider != nullptr) {
        collider->EditorUpdate(dt, this);
    }

    Node::Update(dt);
}

void RigidBody2D::EditorUpdate(float dt)  {
    if (collider != nullptr) {
        collider->EditorUpdate(dt, this);
    }

    Node::EditorUpdate(dt);
}

const glm::vec2 &RigidBody2D::GetVelocity() const {
    return velocity;
}

void RigidBody2D::SetVelocity(const glm::vec2 &velocity_) {
    RigidBody2D::velocity = velocity_;
    if (rawBody != nullptr) {
        this->rawBody->SetLinearVelocity({velocity_.x, velocity_.y});
    }
}

float RigidBody2D::GetAngularDamping() const {
    return angularDamping;
}

void RigidBody2D::SetAngularDamping(float angularDamping_) {
    RigidBody2D::angularDamping = angularDamping_;
}

float RigidBody2D::GetLinearDamping() const {
    return linearDamping;
}

void RigidBody2D::SetLinearDamping(float linearDamping_) {
    RigidBody2D::linearDamping = linearDamping_;
}

float RigidBody2D::GetMass() const {
    return mass;
}

void RigidBody2D::SetMass(float mass_) {
    RigidBody2D::mass = mass_;
    b2MassData massData;
    rawBody->GetMassData(&massData);
    massData.mass = mass_;
    rawBody->SetMassData(&massData);
}

bool RigidBody2D::IsFixedRotation() const {
    return fixedRotation;
}

void RigidBody2D::SetFixedRotation(bool fixedRotation_) {
    RigidBody2D::fixedRotation = fixedRotation_;
    rawBody->SetFixedRotation(fixedRotation_);
}

bool RigidBody2D::IsContinuousCollision() const {
    return continuousCollision;
}

void RigidBody2D::SetContinuousCollision(bool continuousCollision_) {
    RigidBody2D::continuousCollision = continuousCollision_;
    rawBody->SetBullet(continuousCollision_);
}

b2Body *RigidBody2D::GetRawBody() const {
    return rawBody;
}

void RigidBody2D::SetRawBody(b2Body *rawBody_) {
    RigidBody2D::rawBody = rawBody_;
}

BodyType RigidBody2D::GetBodyType() const {
    return bodyType;
}

void RigidBody2D::SetBodyType(BodyType bodyType_) {
    RigidBody2D::bodyType = bodyType_;

    if (rawBody != nullptr) {
        switch (bodyType_) {

        case Static:
            rawBody->SetType(b2_staticBody);
            break;
        case Dynamic:
            rawBody->SetType(b2_dynamicBody);
            break;
        case Kinematic:
            rawBody->SetType(b2_kinematicBody);
            break;
        }
    }

}

ACollider *RigidBody2D::GetCollider() const {
    return collider;
}

void RigidBody2D::SetCollider(ACollider *collider_) {
    delete collider;
    RigidBody2D::collider = collider_;
}

void RigidBody2D::Imgui() {
    Node::Imgui();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(GetNodeType().c_str())) {
        ShowImgui();
    }
}

void RigidBody2D::Start() {
    auto scene = MainWindow::GetScene();
    if (scene != nullptr) {
        scene->GetPhysics2D()->Add(this);
    }
    Node::Start();
}

