#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/Window.hpp"
#include "PropertiesWindow.hpp"

class SceneHierarchyWindow{
public:
    static int selectingUid;

public:
    static void Imgui();

private:
    static void ShowNodeTree();

    static void ShowSubNodes(Node *root);

    static void NodeDropTarget(Node *target);

    static void DummyDropTarget(Node *target);

    static void NodeMenu(Node *node);

    static void ShowAddNodePopup();

    static void ShowNodeDerivedTree(const rttr::type& t, int level);

private:
    static std::string selectedNodeType;

};
