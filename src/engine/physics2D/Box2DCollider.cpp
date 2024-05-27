#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/util/Setting.hpp"

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

    poly->SetAsBox(size.x * 0.5f, size.y * 0.5f,
                   Setting::PHYSICS_SCALE_INV * b2Vec2(origin.x, origin.y), 0);
}

const glm::vec2 &Box2DCollider::GetOrigin() const {
    return origin;
}

void Box2DCollider::SetOrigin(const glm::vec2 &origin_) {
    origin = origin_;

    if (!fixture)
        return;

    auto shape = fixture->GetShape();
    if (!shape)
        return;

    auto poly = dynamic_cast<b2PolygonShape*>(shape);
    if (!poly)
        return;

    poly->SetAsBox(size.x * 0.5f, size.y * 0.5f,
                   Setting::PHYSICS_SCALE_INV * b2Vec2(origin.x, origin.y), 0);
}

void Box2DCollider::EditorUpdate(float dt) {
    if (!fixture) return;
    auto shape = fixture->GetShape();
    if (!shape)
        return;

    auto poly = dynamic_cast<b2PolygonShape*>(shape);
    if (!poly)
        return;
    // 获取矩形的中心
    b2Vec2 center = poly->m_centroid;
    center = Setting::PHYSICS_SCALE * fixture->GetBody()->GetWorldPoint(center);
             // 获取矩形的大小（半宽度和半高度）
    DebugDraw::AddBox2D({center.x, center.y}, size,
                        glm::degrees(fixture->GetBody()->GetAngle()));
}

std::string Box2DCollider::GetColliderShapeType() {
    return "Box2DCollider";
}

json Box2DCollider::Serialize() {
    json j;
    j["type"] = GetColliderShapeType();
    j["size"] = {size.x, size.y};
    j["origin"] = {origin.x, origin.y};
    return j;
}

ASerializableObj *Box2DCollider::Deserialize(json j) {
    auto &s = j["size"];
    if (!s.empty() && s.size() == 2) {
        size = {s[0], s[1]};
    }

    auto &o = j["origin"];
    if (!o.empty() && o.size() == 2) {
        origin = {o[0], o[1]};
    }
    return this;
}

bool Box2DCollider::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Box2DCollider", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawVec2Control("size", size) || res;
        res = MyImGui::DrawVec2Control("origin", origin) || res;
        if (res) {
            SetOrigin(origin);
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
