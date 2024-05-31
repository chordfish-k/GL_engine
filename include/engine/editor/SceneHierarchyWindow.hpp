#pragma once

#include <imgui.h>
#include <vector>
#include "engine/core/Window.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/node/PrefabNode.hpp"

class SceneHierarchyWindow{
public:
    static int selectingUid;
    static PrefabNode * prefabRoot;
    static std::vector<Node *> nodeVisitPath;

public:
    static void Init();

    static void Imgui();

private:
    static void ShowNodeTree();

    static void ShowSubNodes(Node *root);

    static void NodeDropTarget(Node *target);

    static void PrefabDropTarget(Node *target);

    static void DummyDropTarget(Node *target);

    static void NodeMenu(Node *node);

    static void ShowAddNodePopup(Node *node);

    static void ShowNodeDerivedTree(const rttr::type& t, int level);

private:
    static std::string selectedNodeType;

};
