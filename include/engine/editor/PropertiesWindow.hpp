#pragma once

#include "engine/node/Node.hpp"

class PropertiesWindow {
private:
    static Node *activeNode;
    static float debounce;
    static bool remainInThisFrame;

public:
    static void Imgui();

    static void Update(float dt);

    static Node *GetActiveNode();

    static void SetActiveNode(Node *go);

    static void RemainActiveNode(); // 调用这个方法，当前帧的update不再将active设置为nullptr
};


