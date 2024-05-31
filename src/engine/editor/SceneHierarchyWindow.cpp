#include "engine/editor/SceneHierarchyWindow.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/node/IUnselectableNode.hpp"
#include "engine/util/PrefabsUtils.hpp"
#include "engine/util/Common.hpp"
#include "engine/node/PrefabNode.hpp"
#include <filesystem>

#define DISABLE_COLOR {0.2, 0.2, 0.2, 1}

int SceneHierarchyWindow::selectingUid = -1;
std::string SceneHierarchyWindow::selectedNodeType = "Node";
PrefabNode * SceneHierarchyWindow::prefabRoot = nullptr;
std::vector<Node *> SceneHierarchyWindow::nodeVisitPath;

void SceneHierarchyWindow::Init() {
    SceneHierarchyWindow::selectingUid = -1;
    SceneHierarchyWindow::selectedNodeType = "Node";
    SceneHierarchyWindow::prefabRoot = nullptr;
    SceneHierarchyWindow::nodeVisitPath.clear();
    SceneHierarchyWindow::nodeVisitPath.push_back(MainWindow::GetScene()->root);
}

void SceneHierarchyWindow::Imgui() {
    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiStyle styleOrigin = ImGui::GetStyle();
    style.IndentSpacing = 12.0f;            // 设置缩进间距
    style.ItemSpacing = ImVec2(1.0f, 1.0f); // 设置项之间的间距

    auto scene = MainWindow::GetScene();
    if (scene) {
        if (nodeVisitPath.size() == 1) {
            ImGui::Begin("Scene Hierarchy");
            ShowSubNodes(nodeVisitPath[0]);
            ImGui::End();
        }
        /*
        else if (nodeVisitPath.size() > 1){
            ImGui::Begin("Prefab Hierarchy");
            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
            ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, lineHeight);
            if (ImGui::Button("Back", buttonSize)) {
                nodeVisitPath.pop_back();
            }
            auto *fb = dynamic_cast<PrefabNode*>(nodeVisitPath[nodeVisitPath.size()-1]);
            if (fb)
                ShowSubNodes(fb->GetNode());
            ImGui::End();
        }
        */
    }

    style.IndentSpacing = styleOrigin.IndentSpacing; // 设置缩进间距
    style.ItemSpacing = styleOrigin.ItemSpacing; // 设置项之间的间距}

}

void SceneHierarchyWindow::ShowNodeTree() {
    auto root = MainWindow::GetScene()->root;
    auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_FramePadding |
                     ImGuiTreeNodeFlags_OpenOnArrow |
                     ImGuiTreeNodeFlags_SpanAvailWidth;

    auto nodeFlags = baseFlags;
    if (selectingUid == root->GetUid()) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool treeNodeOpen = ImGui::TreeNodeEx(
        (void*)(intptr_t)(root->GetUid()),
        nodeFlags,
        "%s", root->GetName().c_str());

    // select
    if (ImGui::IsItemClicked()) {
        selectingUid = root->GetUid();
        PropertiesWindow::SetActiveNode(root);
    }

    NodeMenu(root);

    NodeDropTarget(root);
    PrefabDropTarget(root);

    if (treeNodeOpen) {
        // sub
        ShowSubNodes(root);
        ImGui::TreePop();
    }
}

void SceneHierarchyWindow::ShowSubNodes(Node *root) {
    auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_OpenOnArrow |
                     ImGuiTreeNodeFlags_FramePadding |
                     ImGuiTreeNodeFlags_SpanAvailWidth;
    auto &obj = root;

    auto nodeFlags = baseFlags;
    if (selectingUid == obj->GetUid()) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    if (obj->children.empty()) {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool treeNodeOpen = ImGui::TreeNodeEx(
        (void*)(intptr_t)(obj->GetUid()),
        nodeFlags,
        "%s", obj->GetName().c_str());

    // select
    if (ImGui::IsItemClicked()) {
        selectingUid = obj->GetUid();
        PropertiesWindow::SetActiveNode(obj);
    }

    // 如果是prefab，则允许双击展开
    if (ImGui::IsItemHovered()
        && ImGui::GetIO().MouseDoubleClicked[GLFW_MOUSE_BUTTON_LEFT]
        && dynamic_cast<PrefabNode*>(obj)) {
//        nodeVisitPath.push_back(dynamic_cast<PrefabNode*>(obj));
        MainWindow::ChangeScene((dynamic_cast<PrefabNode*>(obj))->GetPrefabFile());
    }

    NodeMenu(obj);

    // 拖拽变更子节点相对位置
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("NODE", &obj, sizeof(Node*));
        ImGui::Text("%s", obj->GetName().c_str());
        ImGui::EndDragDropSource();
    }

    NodeDropTarget(obj);
    PrefabDropTarget(obj);
    DummyDropTarget(obj);

    if (treeNodeOpen) {
        for (auto obj_ : obj->children) {
            if (!obj_->IsDoSerialization()) {
                continue;
            }

            // sub
            ShowSubNodes(obj_);
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyWindow::DummyDropTarget(Node *target) {
    // 拖动到缝隙，用于调整顺序
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    // 设置Dummy的大小为剩余可用空间的大小
    ImGui::Dummy(ImVec2(availableSize.x, 1));

    // 拖动到缝隙，用于调整顺序
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Node*));
            Node *source = *((Node **)payload->Data);
            if (source == target) return;

            source->parent->children.remove(source);

            auto it = std::find(target->parent->children.begin(), target->parent->children.end(), target);
            if (it != target->parent->children.end())
                target->parent->children.insert(std::next(it), source);
            else
                target->parent->children.push_back(source);
            source->parent = target->parent;
        }
        ImGui::EndDragDropTarget();
    }
}


void SceneHierarchyWindow::NodeMenu(Node *node) {
    // 右键菜单
    if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered()) {
        // 打开一个弹出式菜单
        selectingUid = node->GetUid();
        if (node->parent != nullptr)
            PropertiesWindow::SetActiveNode(node);
        ImGui::OpenPopup(("Popup for Node" + std::to_string(node->GetUid())).c_str());
    }

    bool openAddNodePopup = false;

    if (ImGui::BeginPopupContextItem(("Popup for Node"+ std::to_string(node->GetUid())).c_str())) {
        // 添加菜单项
        if (ImGui::MenuItem("Add..."))
        {
            // 打开内部弹窗
            openAddNodePopup = true;
        }

        if (ImGui::MenuItem("Copy")) {
            // 复制该节点，json存入剪贴板
            json j = node->Serialize();
            auto jsonText = j.dump(2);
            glfwSetClipboardString(MainWindow::GetGlfwWindow(), jsonText.c_str());
        }

        if (ImGui::MenuItem("Paste")) {
            // 将剪贴板的文本作为json反序列化为节点，作为子节点添加到node下
            auto jsonText = glfwGetClipboardString(MainWindow::GetGlfwWindow());
            auto j = Str2Json(jsonText);
            auto nodePtr = PrefabsUtils::GenerateNodeFromJson(j);
            nodePtr->TravelOnSubTree([](auto n) {
                n->GeneratedId(true);
            });
            MainWindow::GetScene()->AddNodeAsChild(node, nodePtr);
        }
        if (ImGui::MenuItem("Delete")) {
            // 删除该节点
            node->Destroy();
        }
        ImGui::EndPopup();
    }

    if (openAddNodePopup) {
        ImGui::OpenPopup(("Add Node" + std::to_string(node->GetUid())).c_str());
    }
    // 渲染内部弹窗
    ShowAddNodePopup(node);
}

void SceneHierarchyWindow::NodeDropTarget(Node *target) {
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Node*));
            Node *source = *((Node **)payload->Data);

            // 如果 s 是 t 直接子节点，或者 t 是 s 的直接子节点，不做任何事
            if (source->parent != target && target->parent != source) {

                source->transform.ApplyDataByLocalMatrix(
                                              source->GetModelMatrixRelativeTo(target));

                // 将 s 移动到 t 的下面
                source->parent->children.remove(source);
                target->children.push_back(source);
                source->parent = target;
            }
        }
        ImGui::EndDragDropTarget();
    }

}

void SceneHierarchyWindow::PrefabDropTarget(Node *target) {
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
        {
            const char *path_ = ((char *)payload->Data);

            auto prefab = new PrefabNode();
            prefab->SetPrefabFile(path_);
            prefab->Start();
            // 添加prefab到target
            target->AddNode(prefab);
        }
        ImGui::EndDragDropTarget();
    }
}

void SceneHierarchyWindow::ShowAddNodePopup(Node *node) {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(("Add Node" + std::to_string(node->GetUid())).c_str(),
                                NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        auto t = rttr::type::get<Node>();
        ShowNodeDerivedTree(t, 0);

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();

        ImGui::SameLine();
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            auto type = rttr::type::get_by_name(selectedNodeType);
            auto instance = type.create().get_value<Node*>();
            auto parentNode = node;
            parentNode = parentNode == nullptr ? MainWindow::GetScene()->root : parentNode;
            MainWindow::GetScene()->AddNodeAsChild(parentNode, instance);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
void SceneHierarchyWindow::ShowNodeDerivedTree(const rttr::type& t, int level) {

    auto nodeFlags =ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_FramePadding |
                    ImGuiTreeNodeFlags_SpanAvailWidth;

    auto derivedClasses = t.get_derived_classes();

    if (derivedClasses.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    if (level == 0)
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

    // 如果继承了IUnselectable接口，则这个Node不可手动添加，只能作为基节点
    bool selectable = true;
    if (t.is_derived_from<IUnselectableNode>()) {
        selectable = false;
    }

    if (selectedNodeType == t.get_name().to_string() && selectable)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    // 不可选中的颜色
    if (!selectable) ImGui::PushStyleColor(ImGuiCol_Text, DISABLE_COLOR);

    bool open = ImGui::TreeNodeEx(t.get_name().data(),nodeFlags);

    if (!selectable) ImGui::PopStyleColor();

    if (open) {
        if (ImGui::IsItemClicked()) {
            selectedNodeType = t.get_name().to_string();
        }

        for (const auto& derived : derivedClasses){
            ShowNodeDerivedTree(derived, level + 1);
        }
        ImGui::TreePop();
    }

}
