#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/physics2D/Box2DCollider.hpp"

ColliderShape2D::ColliderShape2D() {
    auto coll = new Box2DCollider();
    coll->colliderShape2D = this;
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
    delete collider;
    collider = shape;
}

json ColliderShape2D::Serialize() {
    json j = Node::Serialize();
    if (collider) {
        j["data"]["collider"] = collider->Serialize();
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
        if (t.empty() || !t.is_string()) return this;

        if (t == "Box2DCollider") {
            auto coll = new Box2DCollider();
            coll->Deserialize(c);
            coll->colliderShape2D = this;

            delete collider;
            collider = coll;
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
    if (collider != nullptr && Setting::PHYSICS_DRAW_DEBUG) {
        collider->EditorUpdate(dt);
    }
    Node::Update(dt);
}

void ColliderShape2D::EditorUpdate(float dt) {

    if (!(lastTransform.Equals(transform))) {
        lastTransform = transform;
//        collider.SetOrigin
    }

    if (collider != nullptr) {
        collider->EditorUpdate(dt);
    }
    Node::EditorUpdate(dt);
}
