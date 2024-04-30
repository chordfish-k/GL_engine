#pragma once

#include "engine/node/Node.hpp"

class PropertiesWindow {
private:
    static Node *activeNode;

public:
    static void Imgui() {
        if (activeNode != nullptr) {
            ImGui::Begin("Properties");

            // TODO right click menu

            activeNode->Imgui();
            ImGui::End();
        }
    }

    static Node *GetActiveNode() {
        return activeNode;
    }

    static void SetActiveNode(Node *go) { activeNode = go;
    }
};


