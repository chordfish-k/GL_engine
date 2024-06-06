#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/util/Mat4Utils.hpp"

RigidBody2D::RigidBody2D() {
    mass = 10;
}

RigidBody2D::~RigidBody2D() {
}

void RigidBody2D::Update(float dt)  {
    if (!active) return;

    if (rawBody != nullptr && bodyType == BodyType::Static){
        auto pmat = GetModelMatrix();
        auto p = pmat * glm::vec4(0, 0, 0, 1);
        auto ro = glm::radians(GetTransform().rotation);
        rawBody->SetTransform({Setting::PHYSICS_SCALE_INV * p.x, Setting::PHYSICS_SCALE_INV * p.y}, ro);
    }

    if (rawBody != nullptr && rawBody->IsAwake() && bodyType != BodyType::Static) {
        // 更新rawBody的实际位置到gameObject的transform组件

        Transform temp;
        temp.ApplyDataByLocalMatrix(GetModelMatrix());
        auto scale = transform.scale;
        auto dWorldMat = TransformMatBuilder()
                             .Translate(glm::vec2(rawBody->GetPosition().x,rawBody->GetPosition().y) * Setting::PHYSICS_SCALE)
                             .Rotate(rawBody->GetAngle())
                             .Scale(temp.scale)
                             .Build();

        auto pInvMat = parent ? glm::inverse(parent->GetModelMatrix()) : glm::mat4(1);
        auto dParentMat = pInvMat * dWorldMat;
        transform.ApplyDataByLocalMatrix(dParentMat);
        transform.scale = scale;

        auto vel = rawBody->GetLinearVelocity();
        linear.velocity = glm::vec2(vel.x, vel.y) * Setting::PHYSICS_SCALE;

        auto angV = rawBody->GetAngularVelocity();
        angular.velocity = angV;
    }

    Node::Update(dt);
}

void RigidBody2D::EditorUpdate(float dt)  {
    if (!active) return;

    if (rawBody != nullptr){
        auto pmat = GetModelMatrix();
        auto p = pmat * glm::vec4(0, 0, 0, 1);
        auto ro = glm::radians(GetTransform().rotation);
        rawBody->SetTransform({Setting::PHYSICS_SCALE_INV * p.x, Setting::PHYSICS_SCALE_INV * p.y}, ro);
    }
    Node::EditorUpdate(dt);
}

const glm::vec2 &RigidBody2D::GetLinearVelocity() const {
    return linear.velocity;
}

void RigidBody2D::SetLinearVelocity(const glm::vec2 &velocity_) {
    linear.velocity = velocity_;
    if (rawBody != nullptr) {
        rawBody->SetLinearVelocity(Setting::PHYSICS_SCALE_INV * b2Vec2(velocity_.x, velocity_.y));
    }
}

const float &RigidBody2D::GetAngularVelocity() const {
    return angular.velocity;
}

void RigidBody2D::SetAngularVelocity(const float &velocity_) {
    angular.velocity = velocity_;
    if (rawBody != nullptr) {
        rawBody->SetAngularVelocity(Setting::PHYSICS_SCALE_INV * velocity_);
    }
}

float RigidBody2D::GetAngularDamping() const {
    return angular.damp;
}

void RigidBody2D::SetAngularDamping(float angularDamping_) {
    angular.damp = angularDamping_;
    if (rawBody)
        rawBody->SetAngularDamping(angularDamping_);
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
    if (rawBody != nullptr) {
        rawBody->GetWorld()->DestroyBody(rawBody);
    }
    rawBody = rawBody_;
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
            SetLinearVelocity({linear.velocity.x, linear.velocity.y});
            SetLinearDamping(linear.damp);
        }

        if (angular.Imgui()) {
            SetAngularVelocity(angular.velocity);
            SetAngularDamping(angular.damp);
        }
    }
}

void RigidBody2D::Start() {
    if (!started) {
        auto scene = MainWindow::GetScene();
        if (scene != nullptr) {
            scene->GetPhysics2D()->ReAdd(this);
        }
    }
    Node::Start();
}

json RigidBody2D::Serialize() {
    json j = Node::Serialize();
    j["data"]["mass"] = mass;
    j["data"]["bodyType"] = GetNameByBodyType(bodyType);
    j["data"]["fixRotate"] = fixedRotation;
    j["data"]["continuous"] = continuousCollision;
    j["data"]["gravityScale"] = gravityScale;
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

    auto &gs = data["gravityScale"];
    if (!gs.empty())
        SetGravityScale(gs);

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

void RigidBody2D::SetActive(bool active) {

//    if (active) {
//        rawBody->SetAwake(active);
//        MainWindow::GetScene()->GetPhysics2D()->DestroyNode(this);
//        MainWindow::GetScene()->GetPhysics2D()->Add(this);
//    }
//    else {
//        rawBody->SetAwake(active);
//        MainWindow::GetScene()->GetPhysics2D()->DestroyNode(this);
//    }
    Node::SetActive(active);
}


void RigidBody2D::BindThisToScript(sol::table &table) {
    table["this"] = (RigidBody2D*)this;
}

float RigidBody2D::GetGravityScale() const {
    return gravityScale;
}

void RigidBody2D::SetGravityScale(float gravityScale_) {
    gravityScale = gravityScale_;
    if (rawBody != nullptr) {
        rawBody->SetGravityScale(gravityScale);
    }
}

BEGIN_RTTR_REG(RigidBody2D)
RTTR_CLASS(RigidBody2D)
    .constructor<>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )

    .property("body type", &RigidBody2D::GetBodyType, &RigidBody2D::SetBodyType)
    .property("gravityScale", &RigidBody2D::GetGravityScale, &RigidBody2D::SetGravityScale)
    .property("mass", &RigidBody2D::GetMass, &RigidBody2D::SetMass)
    .property("fixed rotation", &RigidBody2D::IsFixedRotation, &RigidBody2D::SetFixedRotation)
    .property("continuous", &RigidBody2D::IsContinuousCollision, &RigidBody2D::SetContinuousCollision);
END_RTTR_REG(RigidBody2D)
