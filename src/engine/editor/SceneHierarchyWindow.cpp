#include "engine/editor/SceneHierarchyWindow.hpp"

int SceneHierarchyWindow::selectingUid = -1;

void SceneHierarchyWindow::Imgui() {
    ImGui::Begin("Scene Hierarchy");

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiStyle styleOrigin = ImGui::GetStyle();
    style.IndentSpacing = 12.0f; // 设置缩进间距
    style.ItemSpacing = ImVec2(1.0f, 1.0f); // 设置项之间的间距

    auto root = Window::GetScene()->root;
    auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_FramePadding |
                     ImGuiTreeNodeFlags_OpenOnArrow |
                     ImGuiTreeNodeFlags_SpanAvailWidth;


    bool treeNodeOpen = ImGui::TreeNodeEx(
        (void*)(intptr_t)(root->GetUid()),
        baseFlags,
        "%s", root->GetName().c_str());

    NodeDropTarget(root);

    if (treeNodeOpen) {
        // sub
        ShowSubNodes(root);
        ImGui::TreePop();
    }

    style.IndentSpacing = styleOrigin.IndentSpacing; // 设置缩进间距
    style.ItemSpacing = styleOrigin.ItemSpacing; // 设置项之间的间距
    ImGui::End();
}

void SceneHierarchyWindow::ShowSubNodes(Node *root) {
    auto baseFlags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_OpenOnArrow |
                     ImGuiTreeNodeFlags_FramePadding |
                     ImGuiTreeNodeFlags_SpanAvailWidth;

    for (auto obj : root->children) {
        if (!obj->IsDoSerialization()) {
            continue;
        }

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

        // 右键菜单
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            // 打开一个弹出式菜单
            ImGui::OpenPopup("Popup for Node");
        }
        if (ImGui::BeginPopupContextItem("Popup for Node")) {
            NodeMenu(obj);
            ImGui::EndPopup();
        }


        // 拖拽变更子节点相对位置
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("NODE", &obj, sizeof(Node*));
            ImGui::Text("%s", obj->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        NodeDropTarget(obj);

        DummyDropTarget(obj);

        if (treeNodeOpen) {
            // sub
            ShowSubNodes(obj);
            ImGui::TreePop();
        }
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
    // 添加菜单项
    if (ImGui::MenuItem("Copy")) {
        // 复制该节点，json存入剪贴板
        json j = node->Serialize();
        auto jsonText = j.dump(2);
        glfwSetClipboardString(Window::GetGlfwWindow(), jsonText.c_str());
    }
    if (ImGui::MenuItem("Paste")) {
        // 将剪贴板的文本作为json反序列化为节点，作为子节点添加到node下
        auto jsonText = glfwGetClipboardString(Window::GetGlfwWindow());
        auto j = Str2Json(jsonText);
        auto &type = j["type"];

        // 通过反射创建对应Node并序列化
        auto t = rttr::type::get_by_name(type);
        if (t.is_valid()) {
            auto instance = t.create();
            auto nodeClassPtr = rttr::type::get<Node*>();

            if (nodeClassPtr.is_valid()) {
                auto nodePtr = instance.get_value<Node*>();
                nodePtr->Deserialize(j);
                nodePtr->TravelOnSubTree([](auto n) {
                    n->GeneratedId(true);
                });
                Window::GetScene()->AddNodeAsChild(node, nodePtr);
            }
        }
    }
    if (ImGui::MenuItem("Delete")) {
        // 删除该节点
        node->Destroy();
    }
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

                source->transform = source->GetTransformByModelMatrix(
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
