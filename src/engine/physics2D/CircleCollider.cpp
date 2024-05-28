#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/util/Setting.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/core/Camera.hpp"
#include "engine/renderer/DebugDraw.hpp"

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
    }
}

void CircleCollider::EditorUpdate(float dt) {
    if (fixture.empty()) return;

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
    if (!colliderShape2D) return;

    auto center = glm::vec2(0.5f, 0.5f);
    auto modelMat = colliderShape2D->GetModelMatrix();

    segments = 16;
    if (radius >= 30) {
        auto r = radius <= 350 ? radius : 350;
        segments += (int)(radius - 30) / 10;
    }

    float angleIncrement = 2.0f * b2_pi / segments;
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleIncrement;

        glm::vec4 pos = modelMat * glm::vec4(radius * cosf(angle),radius * sinf(angle), 0, 1);
        pos = pos * Setting::PHYSICS_SCALE_INV;
        auto p = fixture[0]->GetBody()->GetLocalPoint({pos.x, pos.y});
        vs[i].Set(p.x, p.y);
    }

    // 分割顶点并创建多个多边形
    glm::vec4 po = Setting::PHYSICS_SCALE_INV * modelMat * glm::vec4(0, 0, 0, 1);
       for (int i = 0; i < segments; i++) {
        b2Vec2 polygonVertices[3] = {b2Vec2(po.x, po.y)};
        int32 vertexCount = 1;

        // 第一个顶点是多边形的中心
//        polygonVertices[vertexCount++] = b2Vec2(0, 0);

        // 复制顶点到多边形
        // TODO 提高单个fixture顶点数
        for (int j = 0; j < 2; ++j) {
            polygonVertices[vertexCount++] = vs[(i + j) % segments];
        }


        // 创建新的多边形形状
        b2PolygonShape poly;//dynamic_cast<b2PolygonShape*>(fixture[i]->GetShape());
        poly.Set(polygonVertices, vertexCount);

//        fixtures[i]
        SetFixture(fixture[0]->GetBody()->CreateFixture(&poly, fixture[0]->GetDensity()), i);
    }

}
