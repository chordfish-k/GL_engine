#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/SceneHierarchyWindow.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/core/Camera.hpp"
#include "engine/renderer/DebugDraw.hpp"

Node *PropertiesWindow::activeNode = nullptr;

float PropertiesWindow::debounce = 0.2f;

bool PropertiesWindow::remainInThisFrame = false;

void PropertiesWindow::Imgui() {
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 5});
//    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0, 0});
    ImGui::Begin("Properties");
    if (activeNode != nullptr) {
        activeNode->Imgui();

        // Transform位置指示圆圈
        auto pSize = MainWindow::GetScene()->GetCamera()->GetProjectionSize();
        auto zoom = MainWindow::GetScene()->GetCamera()->GetZoom();
        auto circleR = 0.008f * zoom * (pSize.x < pSize.y ? pSize.x : pSize.y); // 圆圈半径
        Transform temp;
        temp.ApplyDataByLocalMatrix(activeNode->GetModelMatrix());
        auto center = temp.position;
        DebugDraw::AddCircle(center, circleR);
    }
    ImGui::End();
    ImGui::PopStyleVar(4);
}

Node *PropertiesWindow::GetActiveNode() {
    return activeNode;
}

void PropertiesWindow::SetActiveNode(Node *go) {
    activeNode = go;
}

void PropertiesWindow::EditorUpdate(float dt) {
    if (!GameViewWindow::GetWantCaptureMouse()) return;

    debounce -= dt;
    auto scene = MainWindow::GetScene();
    if (scene == nullptr) return;

    if (MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !remainInThisFrame) {
        int x = (int) MouseListener::GetScreenX();
        int y = (int) MouseListener::GetScreenY();
        int uid = MainWindow::GetPickingTexture()->ReadPixel(x, y)+1;

        Node *pickedNode = nullptr;
        if (uid != 0) {
            pickedNode = scene->GetNodeByUid(uid);
        }

        // 如果当前是TileMap，而拾取的不是，则不执行任何操作
        if (activeNode && activeNode->GetNodeType() == "TileMap"
            && (pickedNode == nullptr || pickedNode->GetNodeType() != "TileMap")) {
            return;
        }

        if (pickedNode) {
            if (pickedNode != nullptr && pickedNode->IsPickable()) {
                activeNode = pickedNode;
            } else if (pickedNode == nullptr && !MouseListener::IsDragging()) {
                activeNode = nullptr;
            }
            debounce = 0.2f;
        }
        else if (!remainInThisFrame){
            activeNode = nullptr;
        }

        // 顺便改变层级树选择的节点
        SceneHierarchyWindow::selectingUid = uid;
    }
    remainInThisFrame = false;
}

void PropertiesWindow::RemainActiveNode() {
    remainInThisFrame = true;
}
