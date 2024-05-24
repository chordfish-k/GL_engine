#include "engine/node/EditorMouseControls.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MainWindow.hpp"

void EditorMouseControls::PickupNode(Node *node) {
    this->holdingNode = node;
    auto activeNode = PropertiesWindow::GetActiveNode();
//    if (activeNode != nullptr) {
//        activeNode->AddNode(node);
//    } else {
    MainWindow::GetScene()->AddNode(node);
//    }
}

void EditorMouseControls::PlaceObject() {
    this->holdingNode = nullptr;
}

void EditorMouseControls::EditorUpdate(float dt){
    Node::Update(dt);
    if (holdingNode == nullptr)
        return;

    auto &t = holdingNode->transform;
    t.position = {
        MouseListener::GetWorldX(), MouseListener::GetWorldY()
    };
    if (holdingNode->parent && holdingNode->parent != MainWindow::GetScene()->root) {

    }
    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        PlaceObject();
    }
};