#include "engine/node/EditorMouseControls.hpp"

void EditorMouseControls::PickupNode(Node *node) {
    this->holdingNode = node;
    Window::GetScene()->AddNode(node);
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
        MouseListener::GetWorldX(),
        MouseListener::GetWorldY()
    };
    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        PlaceObject();
    }
};