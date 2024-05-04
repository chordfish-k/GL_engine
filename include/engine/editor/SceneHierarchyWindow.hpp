#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/Window.hpp"
#include "PropertiesWindow.hpp"

class SceneHierarchyWindow {
public:
    static void Imgui();

    static int ShowSubNodes(Node *root, int indexStart);

    static void DummyDropTarget(Node *target);
};
