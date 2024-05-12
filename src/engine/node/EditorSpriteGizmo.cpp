#include "engine/node/EditorSpriteGizmo.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/Camera.hpp"
#include "engine/editor/GameViewWindow.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

void EditorSpriteGizmo::Update(float dt) {
    lastActiveNode = activeNode;
    activeNode = PropertiesWindow::GetActiveNode();
    if (lastActiveNode != activeNode) {
        selectedPointIndex = -1;
        mode = NONE;
    }

    if (activeNode == nullptr || activeNode->GetNodeType() != "SpriteRenderer") {
        mode = NONE;
        return;
    }

    if (!GameViewWindow::GetWantCaptureMouse()) return;



    DrawBorderAndPoints();

    CheckAndApplyMove();
    CheckAndApplyRotation();
    CheckAndApplyScale();
    Node::Update(dt);
}

void EditorSpriteGizmo::DrawBorderAndPoints() {
    // 画外框
    auto *spr = (SpriteRenderer*)activeNode;
    Transform t = spr->GetTransform();
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


    // 画外框，顺便画圆圈
    auto pSize = Window::GetScene()->GetCamera()->GetProjectionSize();
    auto zoom = Window::GetScene()->GetCamera()->GetZoom();
    circleR = 0.008f * zoom * (pSize.x < pSize.y ? pSize.x : pSize.y); // 圆圈半径
    scaleCircleCenter.clear();
    rotateCircleCenter.clear();

    spriteCenter = (vs[2] + vs[0]) * 0.5f; // 图形中心
    for (int i = 1; i < 5; i++) {
        auto &from = vs[i - 1];
        auto &to = vs[i % 4];
        DebugDraw::AddLine2D(from, to);

        if (zoom < 3) {
            auto cen = (from + to) * 0.5f; // 线条中心
            cen = cen + glm::vec2(glm::normalize(cen)) * circleR; // 往法线方向偏移
            scaleCircleCenter.push_back(cen);
            DebugDraw::AddCircle(cen, circleR);
            if (mode == SCALE && i-1 == selectedPointIndex) { // 同心圆表示强调选中
                DebugDraw::AddCircle(cen, circleR * 0.5f);
            }

            // 顶点的圆圈（旋转用）
            auto n2 = glm::normalize(from - spriteCenter);
            auto cen2 = from;
            cen2 = cen2 + n2 * circleR;
            rotateCircleCenter.push_back(cen2);
            DebugDraw::AddCircle(cen2, circleR, {1, 0, 0});
            if (mode == ROTATE && i-1 == selectedPointIndex) { // 同心圆表示强调选中
                DebugDraw::AddCircle(cen2, circleR * 0.5f, {1, 0, 0});
            }
        }
    }
}

void EditorSpriteGizmo::CheckAndApplyMove() {
    if (!activeNode) return;

    if (MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && mode == NONE) {
        mode = MOVE;
    }

    if (mode == MOVE) {
        if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            auto dp = glm::vec2(MouseListener::GetWorldDx(), MouseListener::GetWorldDy());
            auto dWorldMat = glm::translate(glm::mat4(1), {dp.x, dp.y, 0});
            auto oldWorldMat = activeNode->GetModelMatrix();
            auto newWorldMat = dWorldMat * oldWorldMat;
            auto dParentMat = glm::inverse(activeNode->parent->GetModelMatrix()) * newWorldMat;
            activeNode->transform.position = activeNode->GetTransformByModelMatrix(dParentMat).position;

            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        }
        else {
            mode = NONE;
            selectedPointIndex = -1;
        }
    }
}

void EditorSpriteGizmo::CheckAndApplyRotation() {
    if (!activeNode) return;

    // 检测鼠标是否在某个旋转圈内(附近)
    auto mp = glm::vec2(MouseListener::GetWorldX(), MouseListener::GetWorldY());
    int index = -1;
    bool find = false;
    for (auto &p : rotateCircleCenter) {
        index++;
        auto d = mp - p;
        // 如果是，则进入旋转模式
        if ((d.x*d.x+d.y*d.y) <= circleR*circleR) {
            find = true;
            // 防止选择了另一个物体
            PropertiesWindow::RemainActiveNode();
            // 设置鼠标形状
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            break;
        }
    }

    if (find && mode != ROTATE && mode != SCALE
        && MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        selectedPointIndex = index;
        mode = ROTATE;
    }


    if (mode == ROTATE && selectedPointIndex >= 0) {
        if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            // 鼠标位置可能在Sprite外，要防止目标丢失
            PropertiesWindow::RemainActiveNode();
            // 设置鼠标形状
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

            auto mp = glm::vec2(MouseListener::GetWorldX(), MouseListener::GetWorldY());
            auto dp = glm::vec2(MouseListener::GetWorldDx(), MouseListener::GetWorldDy());

            auto AB = glm::vec3(rotateCircleCenter[selectedPointIndex] - spriteCenter, 0);
            auto AC = glm::vec3(AB + glm::vec3(dp, 0));
            auto axis = glm::normalize(glm::cross(AB, AC));
            auto sign = axis.z;
            auto angle = sign * glm::degrees(glm::orientedAngle(
                       glm::normalize(AB),
                       glm::normalize(AC),
                       axis));

            auto &rotation = activeNode->transform.rotation;

            if (fabs(angle) >= 0.0001)
            rotation += angle;
            if (rotation > 180) rotation = rotation - 360;
            if (rotation < -180) rotation = rotation + 360;

        } else {
            selectedPointIndex = -1;
            mode = NONE;
        }
    }
}

void EditorSpriteGizmo::CheckAndApplyScale() {
    if (!activeNode) return;

    // 检测鼠标是否在某个旋转圈内(附近)
    auto mp = glm::vec2(MouseListener::GetWorldX(), MouseListener::GetWorldY());
    int index = -1;
    bool find = false;
    for (auto &p : scaleCircleCenter) {
        index++;
        auto d = mp - p;
        // 如果是，则进入缩放模式
        if ((d.x*d.x+d.y*d.y) <= circleR*circleR) {
            find = true;
            // 防止选择了另一个物体
            PropertiesWindow::RemainActiveNode();
            // 设置鼠标形状
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            break;
        }
    }

    if (find && mode != ROTATE && mode != SCALE
        && MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        selectedPointIndex = index;
        mode = SCALE;
    }

    if (mode == SCALE && selectedPointIndex >= 0) {
        if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            // 鼠标位置可能在Sprite外，要防止目标丢失
            PropertiesWindow::RemainActiveNode();
            // 设置鼠标形状
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

            auto mp = glm::vec2(MouseListener::GetWorldX(), MouseListener::GetWorldY());
            auto dp = glm::vec2(MouseListener::GetWorldDx(), MouseListener::GetWorldDy());

            auto AB = glm::vec3(scaleCircleCenter[selectedPointIndex] - spriteCenter, 0);
            auto AC = glm::vec3(dp, 0);
            // 检查AB和AC夹角
            float sign = 1;
            auto angle = glm::degrees(glm::orientedAngle(
                glm::normalize(AB), glm::normalize(AC), {0, 0, 1}));
            auto fa = fabs(angle);
            if (fa >= 0.01f) {
                if (fa > 90) sign = -1;

                auto distance = glm::sqrt(dp.x*dp.x + dp.y*dp.y) * 0.3f;
                float value = sign * distance;


                if (selectedPointIndex % 2 == 0)
                    activeNode->transform.scale.x += value / activeNode->GetTransform().scale.x;
                else
                    activeNode->transform.scale.y += value / activeNode->GetTransform().scale.y;
            }
        }
        else {
            mode = NONE;
            selectedPointIndex = -1;
        }
    }
}