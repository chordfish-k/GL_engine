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
    node->Start();
    Node::Start();
}

void PrefabNode::Update(float dt) {
    node->Update(dt);
    Node::Update(dt);
}

void PrefabNode::EditorUpdate(float dt) {
    node->EditorUpdate(dt);
    Node::EditorUpdate(dt);
}

void PrefabNode::Destroy() {
    node->Destroy();
    Node::Destroy();
}

json PrefabNode::Serialize() {
    json j = Node::Serialize();
    j["prefabFile"] = prefabFile;
    return j;
}

Node *PrefabNode::Deserialize(json j) {
    Node::Deserialize(j);
    auto &p = j["prefabFile"];
    prefabFile = p;
    if (!p.empty()) {
        node = PrefabsUtils::GenerateNodeFromJson(util::LoadJsonFromFile(p));
        node->parent = this;
        node->transform.position = {0, 0};
    }
    return this;
}

std::string PrefabNode::GetName() {
    return Node::GetName();
}

void PrefabNode::SetPrefabFile(const std::string &filePath) {
    prefabFile = filePath;
    node = PrefabsUtils::GenerateNodeFromJson(util::LoadJsonFromFile(filePath));
    name = std::filesystem::path(filePath).stem().string();
    SetNode(node);
}

void PrefabNode::Imgui() {
    Node::Imgui();
}
