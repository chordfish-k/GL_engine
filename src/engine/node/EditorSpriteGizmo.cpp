#include "engine/node/EditorSpriteGizmo.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/editor/GameViewWindow.hpp"

void EditorSpriteGizmo::Update(float dt) {

    static bool dragging = false;

    Node *activeNode = PropertiesWindow::GetActiveNode();
    if (activeNode == nullptr || activeNode->GetNodeType() != "SpriteRenderer") {
        dragging = false;
        return;
    }

    auto *spr = (SpriteRenderer*)activeNode;
    Transform t = spr->GetTransform();

    // 画外框
    auto scale = t.scale;
    auto position = t.position;
    auto size = spr->GetSize();
    auto offset_ = spr->GetOffset();
    auto center = spr->IsCentered() ? glm::vec2(0.5f, 0.5f) : glm::vec2(1, 1);
    auto modelMat = spr->GetModelMatrix();

    // 计算外框顶点坐标
    float xAdd = size.x * center.x;
    float yAdd = size.y * center.y;

    std::vector<glm::vec2> vs;
    for (int i = 0; i < 4; i++) {
        if (i == 1) {
            yAdd = size.y * (center.y - 1);
        } else if (i == 2) {
            xAdd = size.x * (center.x - 1);
        } else if (i == 3) {
            yAdd = size.y * center.y;
        }

        auto pos = modelMat * glm::translate(glm::mat4(1), {offset_, 0}) *
                   glm::vec4(xAdd, yAdd, 0, 1);
        vs.emplace_back(pos);
    }

    // 画外框
    DebugDraw::AddLine2D(vs[0], vs[3]);
    for (int i = 1; i < 4; i++) {
        DebugDraw::AddLine2D(vs[i-1], vs[i]);
    }

    if (!GameViewWindow::GetWantCaptureMouse()) return;

    if (MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        dragging = true;
    }

    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && dragging) {

        auto dp = glm::vec2(MouseListener::GetWorldDx(), MouseListener::GetWorldDy());
        auto dWorldMat = glm::translate(glm::mat4(1), {dp.x, dp.y, 0});
        auto oldWorldMat = activeNode->GetModelMatrix();
        auto newWorldMat = dWorldMat * oldWorldMat;
        auto dParentMat = glm::inverse(activeNode->parent->GetModelMatrix()) * newWorldMat;
        activeNode->transform.position = activeNode->GetTransformByModelMatrix(dParentMat).position;

        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
    }
    else {
        dragging = false;
    }

    Node::Update(dt);
}
