#include "engine/node/EditorMouseControls.hpp"
#include "engine/editor/PropertiesWindow.hpp"

void EditorMouseControls::PickupNode(Node *node) {
    this->holdingNode = node;
    auto activeNode = PropertiesWindow::GetActiveNode();
//    if (activeNode != nullptr) {
//        activeNode->AddNode(node);
//    } else {
    Window::GetScene()->AddNode(node);
//    }
}

void EditorMouseControls::PlaceObject() {
    this->holdingNode = nullptr;
}

void EditorMouseControls::Update(float dt){
    Node::Update(dt);
    if (holdingNode == nullptr)
        return;

    auto &t = holdingNode->transform;
    t.position = {
        MouseListener::GetWorldX(), MouseListener::GetWorldY()
    };
    if (holdingNode->parent && holdingNode->parent != Window::GetScene()->root) {

    }
    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        PlaceObject();
    }
};