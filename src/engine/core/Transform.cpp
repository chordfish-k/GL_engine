#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "engine/core/Transform.hpp"
#include "engine/editor/MyImGui.hpp"

Transform::Transform(glm::vec2 position, glm::vec2 scale, float rotation)
    :position(position), scale(scale), rotation(rotation){}


Transform Transform::Copy() const {
    return Transform(position, scale, rotation);
}

void Transform::CopyTo(Transform &to) const {
    to.position = position;
    to.scale = scale;
    to.rotation = rotation;
}

Transform &Transform::operator=(const Transform &t) {
    this->position = t.position;
    this->scale = t.scale;
    this->rotation = t.rotation;
    return *this;
}

bool Transform::Equals(Transform &t) {
    return t.position == this->position &&
           t.scale == this->scale &&
           t.rotation == this->rotation;
}

bool Transform::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        res = MyImGui::DrawVec2Control("position", position, 0, 0.5f);
        res = MyImGui::DrawVec2Control("scale", scale, 0, 0.05f) || res;
        MyImGui::DrawFloatControl("rotation", rotation);
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}
