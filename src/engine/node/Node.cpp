#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"

int Node::ID_COUNTER = 0;

Node::~Node(){
    for (auto &c : children) {
        delete c;
    }
}

void Node::Start() {
    for (auto & i : children) {
        i->Start();
    }
}

void Node::Update(float dt) {
    for (int i = 0; i < children.size(); i++) {
        children[i]->Update(dt);
    }
}

void Node::GeneratedId() {
    if (this->uid == -1) {
        this->uid = ID_COUNTER++;
    }
}

json Node::Serialize() {
    json j;
    j["type"] = GetNodeType();
    auto &pos = transform.position;
    auto &scale = transform.scale;
    auto &rotation = transform.rotation;
    j["name"] = GetName();
    j["data"]["transform"]["position"] = {pos.x, pos.y};
    j["data"]["transform"]["scale"] = {scale.x, scale.y};
    j["data"]["transform"]["rotation"] = rotation;
    for (int i = 0; i < children.size(); i++) {
        j["children"][i] = children[i]->Serialize();
    }
    return j;
};

Node *Node::Deserialize(json j){
    auto &n = j["name"];
    if (!n.empty())
        name = n;

    auto &data = j["data"];
    if (!data.empty()) {

        auto &t = data["transform"];
        if (!t.empty()) {
            auto &pos = t["position"];
            if (!pos.empty())
                transform.position = {pos[0], pos[1]};

            auto &scale = t["scale"];
            if (!scale.empty())
                transform.scale = {scale[0], scale[1]};

            auto &rotation = t["rotation"];
            if (!rotation.empty()) {
                transform.rotation = rotation;
            }

            auto &offset = t["offset"];
            if (!offset.empty()) {
                transform.offset = {offset[0], offset[1]};
            }
        }
    }

    auto &c = j["children"];
    if (!c.empty()) {
        for (auto &ch : c) {
            std::string type = ch["type"];
            if (type.empty())
                continue;

            // 通过反射创建对应Node并序列化
            auto t = rttr::type::get_by_name(type);
            if (t.is_valid()) {
                auto instance = t.create();
                auto nodeClassPtr = rttr::type::get<Node*>();

                if (nodeClassPtr.is_valid()) {
                    auto nodePtr = instance.get_value<Node*>();
                    nodePtr->Deserialize(ch);
                    if (nodePtr)
                        Window::GetScene()->AddNodeAsChild(this,nodePtr);
                }
            }
        }
    }

    return this;
}

void Node::TravelOnSubTree(std::function<void(Node*)> func) {
    func(this);
    for (auto n : children) {
        n->TravelOnSubTree(func);
    }
}

void Node::Imgui() {
    ShowTransformProperties();
}

Node *Node::AddNode(Node *comp) {
    children.push_back(comp);
    comp->parent = this;
    return comp;
}

void Node::RemoveAllNodes() {
    for (auto n : children) {
        n->Destroy();
    }
}

void Node::ShowTransformProperties() {
    //每个Node都有的Transform
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Transform")) {
        auto position = transform.position;
        if (ImGui::DragFloat2("position", glm::value_ptr(position), 0.5f)) {
            transform.position = position;
        }

        auto scale = transform.scale;
        if (ImGui::DragFloat2("scale", glm::value_ptr(scale), 0.05f)) {
            transform.scale = scale;
        }

        auto rotation = transform.rotation;
        if (ImGui::DragFloat("rotation", &rotation)) {
            transform.rotation = rotation;
        }

        auto offset = transform.offset;
        if (ImGui::DragFloat2("offset", glm::value_ptr(offset), 0.05f)) {
            transform.offset = offset;
        }
        ImGui::TreePop();
    }
}

glm::mat4 Node::GetModelMatrix() {
    auto modelMat = glm::mat4(1.f);
    Node *n = this;

    while (n != nullptr) {
        Transform &t = n->transform;
        auto tt = n->GetTransform();
        auto M = glm::mat4(1.f);
        M = glm::translate(M, {t.position.x + tt.scale.x * t.offset.x, t.position.y + tt.scale.y * t.offset.y, 0});
        M = glm::rotate(M, glm::radians(t.rotation), {0, 0, 1});
        M = glm::translate(M, {-tt.scale.x * t.offset.x , -tt.scale.y * t.offset.y , 0});

        modelMat = M * modelMat;

        n = n->parent;
    }
    auto tt = GetTransform();
    modelMat = glm::scale(modelMat, {tt.scale.x, tt.scale.y, 1});

    return modelMat;
}
