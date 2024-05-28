#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/util/Setting.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/core/Camera.hpp"

const glm::vec2 &Box2DCollider::GetSize() const {
    return size;
}

void Box2DCollider::SetSize(const glm::vec2 &size_) {
    size = size_;

    if (!fixture)
        return;

    auto shape = fixture->GetShape();
    if (!shape)
        return;

    auto poly = dynamic_cast<b2PolygonShape*>(shape);
    if (!poly)
        return;

    poly->SetAsBox(size.x * 0.5f, size.y * 0.5f,poly->m_centroid, 0);
}


void Box2DCollider::EditorUpdate(float dt) {
    if (!fixture) return;
    auto shape = fixture->GetShape();
    if (!shape)
        return;

    auto poly = dynamic_cast<b2PolygonShape*>(shape);
    if (!poly)
        return;

    // 画出碰撞体包围盒
    for (int i=0; i<poly->m_count; i++) {
        int index = (i+1)%poly->m_count;
        b2Vec2 vPosA = {poly->m_vertices[i].x, poly->m_vertices[i].y};
        b2Vec2 vPosB = {poly->m_vertices[index].x, poly->m_vertices[index].y};
        auto vA = Setting::PHYSICS_SCALE * fixture->GetBody()->GetWorldPoint(vPosA);
        auto vB = Setting::PHYSICS_SCALE * fixture->GetBody()->GetWorldPoint(vPosB);
        DebugDraw::AddLine2D({vA.x, vA.y}, {vB.x, vB.y});
    }
}

std::string Box2DCollider::GetColliderShapeType() {
    return "Box2DCollider";
}

json Box2DCollider::Serialize() {
    json j;
    j["type"] = GetColliderShapeType();
    j["size"] = {size.x, size.y};
    return j;
}

ASerializableObj *Box2DCollider::Deserialize(json j) {
    auto &s = j["size"];
    if (!s.empty() && s.size() == 2) {
        size = {s[0], s[1]};
    }
    return this;
}

bool Box2DCollider::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Box2DCollider", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawVec2Control("size", size) || res;
        if (res) {
            SetSize(size);
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
