#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/SceneHierarchyWindow.hpp"
#include "engine/core/MainWindow.hpp"

Node *PropertiesWindow::activeNode = nullptr;

float PropertiesWindow::debounce = 0.2f;

bool PropertiesWindow::remainInThisFrame = false;

void PropertiesWindow::Imgui() {
    ImGui::Begin("Properties");
    if (activeNode != nullptr) {
        activeNode->Imgui();
    }
    ImGui::End();
}

Node *PropertiesWindow::GetActiveNode() {
    return activeNode;
}

void PropertiesWindow::SetActiveNode(Node *go) {
    activeNode = go;
}

void PropertiesWindow::Update(float dt) {
    if (!GameViewWindow::GetWantCaptureMouse()) return;

    debounce -= dt;
    auto scene = MainWindow::GetScene();
    if (scene == nullptr) return;

    if (MouseListener::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !remainInThisFrame) {
        int x = (int) MouseListener::GetScreenX();
        int y = (int) MouseListener::GetScreenY();
        int uid = MainWindow::GetPickingTexture()->ReadPixel(x, y)+1;

        // 顺便改变层级树选择的节点
        SceneHierarchyWindow::selectingUid = uid;

        if (uid != 0) {
            Node *pickedNode = scene->GetNodeByUid(uid);
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
    }
    remainInThisFrame = false;
}

void PropertiesWindow::RemainActiveNode() {
    remainInThisFrame = true;
}
