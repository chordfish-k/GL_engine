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

    DummyDropTarget(root);

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
            // 添加菜单项
            if (ImGui::MenuItem("Delete")) {
                // 删除该节点
                obj->Destroy();
            }
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
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Node*));
                Node *source = *((Node **)payload->Data);
                Node *target = obj;

                // 如果 s 是 t 直接子节点，或者 t 是 s 的直接子节点，不做任何事
                if (source->parent != target && target->parent != source) {

                    // 根据 s 的真实变换计算s相对t的相对变换
                    auto sT = source->GetTransform();
                    auto tT = target->GetTransform();
                    Transform t;
                    t.position = sT.position - tT.position;
                    t.scale = sT.scale / tT.scale;
                    t.rotation = sT.rotation - tT.rotation;
                    source->transform = t;

                    // 将 s 移动到 t 的下面
                    source->parent->children.remove(source);
                    target->children.push_back(source);
                    source->parent = target;
                }
            }
            ImGui::EndDragDropTarget();
        }

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