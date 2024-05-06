#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/SceneHierarchyWindow.hpp"

Node *PropertiesWindow::activeNode = nullptr;

float PropertiesWindow::debounce = 0.2f;

void PropertiesWindow::Imgui() {
    if (activeNode != nullptr) {
        ImGui::Begin("Properties");

        activeNode->Imgui();
        ImGui::End();
    }
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
    auto scene = Window::GetScene();

    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        int x = (int) MouseListener::GetScreenX();
        int y = (int) MouseListener::GetScreenY();
        int uid = Window::GetPickingTexture()->ReadPixel(x, y)+1;

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
    }
}
