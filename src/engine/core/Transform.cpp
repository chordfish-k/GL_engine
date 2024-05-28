#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
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

Transform Transform::operator+(Transform b) const {
    Transform res;
    res.position = position + b.position;
    res.scale = scale * b.scale;
    res.rotation = rotation + b.rotation;
    return res;
}

glm::mat4 Transform::GetLocalMatrix() const {
    // 构建当前节点的变换矩阵
    auto M = glm::mat4(1);
    M = glm::translate(M, {position, 0});
    M = glm::rotate(M, glm::radians(rotation), {0,0,1});
    M = glm::scale(M, {scale, 1});
    return M;
}

void Transform::ApplyDataByLocalMatrix(const glm::mat4 &mat) {
    glm::vec3 newPosition, newScale, skew;
    glm::vec4 perspective;
    glm::quat newRotation;
    glm::decompose(mat, newScale, newRotation, newPosition, skew, perspective);

    glm::vec3 eulerAngleRadians = glm::eulerAngles(newRotation);
    glm::vec3 eulerAngleDegrees = glm::degrees(eulerAngleRadians);

    // 将新的变换数值赋值给transform
    position = newPosition;
    scale = newScale;
    rotation = eulerAngleDegrees.z;
}
