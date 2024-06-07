#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/physics2D/CircleCollider.hpp"

ColliderShape2D::ColliderShape2D() {
    auto coll = new Box2DCollider(this);
    collider = coll;
};

ColliderShape2D::~ColliderShape2D(){
    delete collider;
};

void ColliderShape2D::Start() {
    auto scene = MainWindow::GetScene();
    if (scene != nullptr) {
        scene->GetPhysics2D()->Add(this);
    }
    Node::Start();
}

ACollider *ColliderShape2D::GetCollider() const {
    return collider;
}

void ColliderShape2D::SetCollider(ACollider *shape) {
    auto fs = collider->GetFixture();
    if (fs.empty())
        return;
    // 移除当前碰撞体的所以fixture
    MainWindow::GetScene()->GetPhysics2D()->DestroyNode(this);
    delete collider;
    collider = shape;
    MainWindow::GetScene()->GetPhysics2D()->Add(this);
}

json ColliderShape2D::Serialize() {
    json j = Node::Serialize();
    if (collider) {
        j["data"]["collider"] = collider->Serialize();
        j["data"]["collider"]["type"] = GetNameByShapeType(shapeType);
    }
    return j;
}

Node *ColliderShape2D::Deserialize(json j) {
    Node::Deserialize(j);
    auto &data = j["data"];
    if (data.empty()) return this;

    auto &c = data["collider"];
    if (!c.empty()) {
        auto &t = c["type"];
        if (!t.empty()) {
            shapeType = GetShapeTypeByName(t);
            if (t == "Box2DCollider") {
                auto coll = new Box2DCollider(this);
                coll->Deserialize(c);
                delete collider;
                collider = coll;
            }
            else if (t == "CircleCollider") {
                auto coll = new CircleCollider(this);
                coll->Deserialize(c);
                delete collider;
                collider = coll;
            }
        }
    }
    return this;
}

void ColliderShape2D::Imgui() {
    Node::Imgui();
    ImGui::Dummy({1, 5});
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(GetNodeType().c_str())) {
        ShowImgui();
        collider->Imgui();
    }
}

void ColliderShape2D::Update(float dt) {
    if (!active) return;

    TryReshape();

    Node::Update(dt);
}

void ColliderShape2D::EditorUpdate(float dt) {
    if (!active) return;

    TryReshape();

    if (collider != nullptr) {
        collider->RefreshShape();
        collider->EditorUpdate(dt);
    }
    Node::EditorUpdate(dt);
}

void ColliderShape2D::TryReshape() {
    Transform newTr = GetTransform();
    glm::vec2 newPos = transform.position;
    newTr.position = transform.position;
    if (collider) {
        if (fabs(lastTransform.scale.x - newTr.scale.x) > 0.0001 && fabs(lastTransform.scale.y - newTr.scale.y) > 0.0001
            || fabs(lastTransform.rotation - newTr.rotation) > 0.0001 && fabs(newTr.scale.x - newTr.scale.y) > 0.0001
            || lastTransform.position != newTr.position
            ) {
            collider->RefreshShape();
        }
        if (collider->IsDirty()) {
            collider->RefreshShape();
        }
    }
    newTr.CopyTo(lastTransform);
}

ShapeType ColliderShape2D::GetShapeType() const {
    return shapeType;
}

void ColliderShape2D::SetShapeType(ShapeType shapeType_) {
    shapeType = shapeType_;
    ACollider *coll = nullptr;
    if (shapeType_ == ShapeType::Box2DCollider) {
        coll = new Box2DCollider(this);
    } else if (shapeType_ == ShapeType::CircleCollider) {
        coll = new CircleCollider(this);
    }
    SetCollider(coll);
}

RigidBody2D *ColliderShape2D::GetRigidBody2D() const {
    return rigidBody2D;
}

void ColliderShape2D::SetRigidBody2D(RigidBody2D *rigidBody2D) {
    ColliderShape2D::rigidBody2D = rigidBody2D;
}

void ColliderShape2D::BindThisToScript(sol::table &table) {
    table["this"] = (ColliderShape2D*)this;
}

bool ColliderShape2D::IsSensor() const {
    return sensor;
}

void ColliderShape2D::SetSensor(bool sensor_) {
    if (sensor != sensor_) {
        sensor = sensor_;
        for (auto f : collider->GetFixture()) {
            if (f) f->SetSensor(sensor_);
        }
    }
}

BEGIN_RTTR_REG(ColliderShape2D)
RTTR_CLASS(ColliderShape2D)
    .constructor<>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )
    .property("shape", &ColliderShape2D::GetShapeType, &ColliderShape2D::SetShapeType);
END_RTTR_REG(ColliderShape2D)
