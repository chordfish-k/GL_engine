#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/Window.hpp"
#include "PropertiesWindow.hpp"

class SceneHierarchyWindow {
public:
    static int selectingUid;
public:
    static void Imgui();

    static void ShowSubNodes(Node *root);

    static void DummyDropTarget(Node *target);
};
