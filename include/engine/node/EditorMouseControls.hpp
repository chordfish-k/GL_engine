#pragma once

#include "engine/node/Node.hpp"
#include "engine/core/MouseListener.hpp"

class EditorMouseControls : public Node {
    COMPONENT(EditorMouseControls)
protected:
    Node *holdingNode = nullptr;

public:
    void PickupNode(Node *node);

    void PlaceObject();

    void EditorUpdate(float dt) override;
};
