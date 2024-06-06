#include "engine/node/PrefabNode.hpp"
#include "engine/util/PrefabsUtils.hpp"
#include "engine/util/Common.hpp"

PrefabNode::PrefabNode() {}

PrefabNode::~PrefabNode() {
    delete node;
}

void PrefabNode::SetNode(Node *node_) {
    node = node_;
    node->parent = this;
    node->transform.position = {0, 0};
}

Node *PrefabNode::GetNode() {
    return node;
}

void PrefabNode::Start() {
    if (node) {
        node->Start();
    }
    Node::Start();
}

void PrefabNode::Update(float dt) {
    if (node) {
        node->Update(dt);
    }
    Node::Update(dt);
}

void PrefabNode::EditorUpdate(float dt) {
    if (node) {
        node->EditorUpdate(dt);
    }
    Node::EditorUpdate(dt);
}

void PrefabNode::Destroy() {
    if (node) {
        node->Destroy();
    }
    Node::Destroy();
}

json PrefabNode::Serialize() {
    json j = Node::Serialize();
    j["prefabFile"] = util::PathRelativeToProjectRoot(prefabFile);
    return j;
}

Node *PrefabNode::Deserialize(json j) {
    Node::Deserialize(j);
    auto &p = j["prefabFile"];
    prefabFile = p;
    if (!p.empty()) {
        auto jf = util::LoadJsonFromFile(util::GetAbsolutePath(p));
        node = PrefabsUtils::GenerateNodeFromJson(jf);
        if (node) {
            node->parent = this;
            node->Start();
//            node->transform.position = {0, 0};
        }else {
            return nullptr;
        }
    }
    return this;
}

const std::string &PrefabNode::GetPrefabFile() const {
    return prefabFile;
}

std::string PrefabNode::GetName() {
    return Node::GetName();
}

void PrefabNode::SetPrefabFile(const std::string &filePath) {
    prefabFile = util::GetAbsolutePath(filePath);
    node = PrefabsUtils::GenerateNodeFromJson(util::LoadJsonFromFile(prefabFile));
    name = std::filesystem::path(filePath).stem().string();
    SetNode(node);
}

void PrefabNode::SetActive(bool active_) {
    if (active != active_) {
        if (node) node->SetActive(active_);
        active = active_;
    }
}

void PrefabNode::Imgui() {
    node->Imgui();
}

void PrefabNode::BindThisToScript(sol::table &table) {
    table["this"] = (PrefabNode*)this;
}


BEGIN_RTTR_REG(PrefabNode)
RTTR_CLASS(PrefabNode)
    .constructor<>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
    );
END_RTTR_REG(PrefabNode)