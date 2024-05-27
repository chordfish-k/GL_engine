#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/physics2D/Box2DCollider.hpp"

RigidBody2D::RigidBody2D() {
    mass = 10;
}

RigidBody2D::~RigidBody2D() {
}

void RigidBody2D::Update(float dt)  {
    if (rawBody != nullptr) {
        // 更新rawBody的实际位置到gameObject的transform组件
        Transform tr;
        tr.position = {rawBody->GetPosition().x * Setting::PHYSICS_SCALE,
                              rawBody->GetPosition().y * Setting::PHYSICS_SCALE};
        tr.rotation = (float) glm::degrees(rawBody->GetAngle());

        if (parent) {
            auto ptr = parent->transform;
            transform.position = tr.position - ptr.position;
            transform.rotation = tr.rotation - ptr.rotation;
        }
        else {
            transform.position = tr.position;
            transform.rotation = tr.rotation;
        }

        auto vel = rawBody->GetLinearVelocity();
        linear.velocity = glm::vec2(vel.x, vel.y) * Setting::PHYSICS_SCALE;
    }

    Node::Update(dt);
}

void RigidBody2D::EditorUpdate(float dt)  {
    if (rawBody != nullptr){
        auto tr = GetTransform();
        auto pos = Setting::PHYSICS_SCALE_INV * b2Vec2(tr.position.x, tr.position.y);
        auto ro = glm::radians(tr.rotation);
        rawBody->SetTransform(pos, ro);
    }
    Node::EditorUpdate(dt);
}

const glm::vec2 &RigidBody2D::GetLinearVelocity() const {
    return linear.velocity;
}

void RigidBody2D::SetLinearVelocity(const glm::vec2 &velocity_) {
    linear.velocity = velocity_;
    if (rawBody != nullptr) {
        rawBody->SetLinearVelocity(Setting::PHYSICS_SCALE_INV* b2Vec2(velocity_.x, velocity_.y));
    }
}

const float &RigidBody2D::GetAngularVelocity() const {
    return angular.velocity;
}

void RigidBody2D::SetAngularVelocity(const float &velocity_) {
    angular.velocity = velocity_;
    if (rawBody != nullptr) {
        rawBody->SetAngularVelocity(velocity_);
    }
}

float RigidBody2D::GetAngularDamping() const {
    return angular.damp;
}

void RigidBody2D::SetAngularDamping(float angularDamping_) {
    angular.damp = angularDamping_;
}

float RigidBody2D::GetLinearDamping() const {
    return linear.damp;
}

void RigidBody2D::SetLinearDamping(float linearDamping_) {
    linear.damp = linearDamping_;
    if (rawBody)
        rawBody->SetLinearDamping(linearDamping_);
}

float RigidBody2D::GetMass() const {
    return mass;
}

void RigidBody2D::SetMass(float mass_) {
    RigidBody2D::mass = mass_;
    b2MassData massData;
    if (rawBody) {
        rawBody->GetMassData(&massData);
        massData.mass = mass_;
        rawBody->SetMassData(&massData);
    }
}

bool RigidBody2D::IsFixedRotation() const {
    return fixedRotation;
}

void RigidBody2D::SetFixedRotation(bool fixedRotation_) {
    RigidBody2D::fixedRotation = fixedRotation_;
    if (rawBody)
        rawBody->SetFixedRotation(fixedRotation_);
}

bool RigidBody2D::IsContinuousCollision() const {
    return continuousCollision;
}

void RigidBody2D::SetContinuousCollision(bool continuousCollision_) {
    RigidBody2D::continuousCollision = continuousCollision_;
    if (rawBody)
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

        case BodyType::Static:
            rawBody->SetType(b2_staticBody);
            break;
        case BodyType::Dynamic:
            rawBody->SetType(b2_dynamicBody);
            break;
        case BodyType::Kinematic:
            rawBody->SetType(b2_kinematicBody);
            break;
        }
    }

}

void RigidBody2D::Imgui() {
    Node::Imgui();
    ImGui::Dummy({1, 5});
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(GetNodeType().c_str())) {
        ShowImgui();

        if (linear.Imgui()) {
            rawBody->SetLinearVelocity({linear.velocity.x, linear.velocity.y});
            rawBody->SetLinearDamping(linear.damp);
        }

        if (angular.Imgui()) {
            rawBody->SetAngularVelocity(angular.velocity);
            rawBody->SetAngularDamping(angular.damp);
        }
    }
}

void RigidBody2D::Start() {
    auto scene = MainWindow::GetScene();
    if (scene != nullptr) {
        scene->GetPhysics2D()->Add(this);
    }
    Node::Start();
}

json RigidBody2D::Serialize() {
    json j = Node::Serialize();
    j["data"]["mass"] = mass;
    j["data"]["bodyType"] = GetNameByBodyType(bodyType);
    j["data"]["fixRotate"] = fixedRotation;
    j["data"]["continuous"] = continuousCollision;
    j["data"]["linear"]["velocity"] = {linear.velocity.x, linear.velocity.y};
    j["data"]["linear"]["damp"] = linear.damp;
    j["data"]["angular"]["velocity"] = angular.velocity;
    j["data"]["angular"]["damp"] = angular.damp;
    return j;
}

RigidBody2D *RigidBody2D::Deserialize(json j) {
    Node::Deserialize(j);
    auto &data = j["data"];
    if (data.empty()) return this;

    auto &m = data["mass"];
    if (!m.empty())
        SetMass(m);

    auto &t = data["bodyType"];
    if (!t.empty())
        SetBodyType(GetBodyTypeByName(t));

    auto &fr = data["fixRotate"];
    if (!fr.empty())
        SetFixedRotation(fr);

    auto &co = data["continuous"];
    if (!co.empty())
        SetContinuousCollision(co);

    auto &l = data["linear"];
    if (!l.empty()) {
        auto &v = l["velocity"];
        auto &d = l["damp"];
        if (!v.empty() && v.size() == 2)
            SetLinearVelocity({v[0], v[1]});
        if (!d.empty())
            SetLinearDamping(d);
    }

    auto &a = data["angular"];
    if (!a.empty()) {
        auto &v = a["velocity"];
        auto &d = a["damp"];
        if (!v.empty())
            SetAngularVelocity(v);
        if (!d.empty())
            SetAngularDamping(d);
    }
    return this;
}
