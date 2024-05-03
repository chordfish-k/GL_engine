#pragma once

#include "Node.hpp"
#include "engine/core/MouseListener.hpp"

COMPONENT(MouseControls)
protected:
    Node *holdingNode = nullptr;

public:
    void PickupNode(Node *node) {
        this->holdingNode = node;
        Window::GetScene()->AddNode(node);
    }

    void PlaceObject() {
        this->holdingNode = nullptr;
    }

    void Update(float dt) override {
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
};
