#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/util/Setting.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/core/Camera.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/util/Mat4Utils.hpp"

CircleCollider::CircleCollider(ColliderShape2D *collider) {
    colliderShape2D = collider;
    fixture = std::vector<b2Fixture*>(CIRCLE_SEGMENTS);
}

float CircleCollider::GetRadius() const {
    return radius;
}

void CircleCollider::SetRadius(float radius_) {
    if (radius != radius_) {
        radius = radius_;
        SetDirty(true);
    }
}

void CircleCollider::EditorUpdate(float dt) {
    if (fixture.empty() || !fixture[0]) return;

    for (int i = 0; i < segments; i++) {
        // 画出碰撞体包围盒
        int index = (i+1)%segments;
        auto vA = Setting::PHYSICS_SCALE * fixture[0]->GetBody()->GetWorldPoint(vs[i]);
        auto vB = Setting::PHYSICS_SCALE * fixture[0]->GetBody()->GetWorldPoint(vs[index]);
        DebugDraw::AddLine2D({vA.x, vA.y}, {vB.x, vB.y});
    }
}


json CircleCollider::Serialize() {
    json j;
    j["radius"] = radius;
    return j;
}

ASerializableObj *CircleCollider::Deserialize(json j) {
    auto &r = j["radius"];
    if (!r.empty() && r.is_number()) {
        radius = r;
    }
    return this;
}

bool CircleCollider::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("CircleCollider", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        auto r = radius;
        res = MyImGui::DrawFloatControl("radius", r) || res;
        res = res && r >= 1;
        if (res) {
            SetRadius(r);
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}

void CircleCollider::RefreshShape() {
    if (!colliderShape2D || fixture.empty() || !fixture[0]) return;

    auto modelMat = colliderShape2D->GetModelMatrix();

    segments = CIRCLE_SEGMENTS;
//    if (radius >= 30) {
//        auto r = radius <= 350 ? radius : 350;
//        segments += (int)(r - 30) / 10;
//    }

    float angleIncrement = 2.0f * b2_pi / segments;
    glm::vec2 center = colliderShape2D->transform.position * Setting::PHYSICS_SCALE_INV;
    vs[segments+1] = {center.x, center.y};
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleIncrement;
        float xAdd = radius * cosf(angle);
        float yAdd = radius * sinf(angle);
        auto pos = util::TransformPoint(modelMat, {xAdd, yAdd})
            * Setting::PHYSICS_SCALE_INV;
        auto p = colliderShape2D->GetRigidBody2D()
                     ->GetRawBody()->GetLocalPoint({pos.x, pos.y});
        vs[i].Set(p.x, p.y);
    }

    // 分割顶点并创建多个多边形
    for (int i = 0; i < segments; i++) {
        b2Vec2 polygonVertices[3];
        polygonVertices[0] = vs[segments+1];
        int32 vertexCount = 1;

        // 复制顶点到多边形
        // TODO 提高单个fixture顶点数
        for (int j = 0; j < 2; ++j) {
            polygonVertices[vertexCount] = vs[(i + j) % segments];
            vertexCount++;
        }

        // 创建新的多边形形状
        b2PolygonShape poly;
        poly.Set(polygonVertices, vertexCount);

        SetFixture(colliderShape2D->GetRigidBody2D()->GetRawBody()
                       ->CreateFixture(&poly,
                                       colliderShape2D->GetRigidBody2D()->GetMass()), i);

    }
    SetDirty(false);
}

BEGIN_RTTR_REG(CircleCollider)
rttr::registration::class_<CircleCollider>("CircleCollider")
    .constructor<ColliderShape2D*>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
    );
END_RTTR_REG(CircleCollider)