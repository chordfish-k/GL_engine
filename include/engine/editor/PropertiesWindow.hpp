#pragma once

#include "engine/node/Node.hpp"

class PropertiesWindow {
private:
    static Node *activeNode;
    static float debounce;

public:
    static void Imgui();

    static void Update(float dt);

    static Node *GetActiveNode();

    static void SetActiveNode(Node *go);
};


