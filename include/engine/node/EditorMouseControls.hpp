#pragma once

#include "Node.hpp"
#include "engine/core/MouseListener.hpp"

COMPONENT(EditorMouseControls)
protected:
    Node *holdingNode = nullptr;

public:
    void PickupNode(Node *node);

    void PlaceObject();

    void Update(float dt) override;
};
