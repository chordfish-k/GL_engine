#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MainWindow.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

int Node::ID_COUNTER = 0;

Node::~Node(){
    for (auto &c : children) {
        delete c;
    }
    if (PropertiesWindow::GetActiveNode() == this) {
        PropertiesWindow::SetActiveNode(nullptr);
    }
}

void Node::Start() {
    for (auto & i : children) {
        i->Start();
    }
}

void Node::Update(float dt) {
    for (auto go : children) {
        go->Update(dt);
    }
}

void Node::CheckDelete() {
    auto renderer = MainWindow::GetScene()->GetRenderer();
    auto &ch = children;

    for (auto it = ch.begin(); it != ch.end();) {
        auto go = (*it);

        go->CheckDelete();

        if (go->ShouldDestroy()) {
            renderer->DestroyNode(go);
            it = ch.erase(it);
            delete go;
        } else {
            ++it;
        }
    }
}

void Node::GeneratedId(bool force) {
    if (!IsDoSerialization()) return;
    if (force || this->uid == -1) {
        this->uid = ID_COUNTER++;
    }
}

json Node::Serialize() {
    json j;
    if (IsDoSerialization()) {
        j["type"] = GetNodeType();
        auto &pos = transform.position;
        auto &scale = transform.scale;
        auto &rotation = transform.rotation;
        j["name"] = GetName();
        j["data"]["transform"]["position"] = {pos.x, pos.y};
        j["data"]["transform"]["scale"] = {scale.x, scale.y};
        j["data"]["transform"]["rotation"] = rotation;
        j["data"]["zIndex"] = zIndex.GetZIndex();
        int i = 0;
        for (auto n : children) {
            auto sub = n->Serialize();
            if (sub.empty())
                continue;
            j["children"][i++] = sub;
        }
    }

    return j;
};

Node *Node::Deserialize(json j){
    GeneratedId(true);

    auto &n = j["name"];
    if (!n.empty())
        SetName(n);

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
        }

        auto &z = data["zIndex"];
        if (!z.empty()) {
            zIndex = z;
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
                        MainWindow::GetScene()->AddNodeAsChild(this,nodePtr);
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

bool Node::IsChildOf(Node *p) {
    if (!parent) return false;
    if (parent == p) return true;
    return parent->IsChildOf(p);
}

void Node::Imgui() {
    ShowNodeProperties();
}

Node *Node::AddNode(Node *comp) {
    children.push_back(comp);
    comp->parent = this;
    comp->GeneratedId();
    return comp;
}

void Node::RemoveAllNodes() {
    for (auto n : children) {
        n->Destroy();
    }
}

bool Node::ShouldDestroy() const {
    return shouldDestroy;
}

bool Node::IsPickable() const {
    return isPickable;
}

void Node::SetIsPickable(bool isPickable) {
    Node::isPickable = isPickable;
}

void Node::ShowNodeProperties() {
    //每个Node都有的Transform
    transform.Imgui();
}

glm::mat4 Node::GetModelMatrix() {
    // 构建当前节点的变换矩阵
    auto M = glm::mat4(1);

    auto t = GetTransform();

    M = glm::translate(M, {transform.position, 0});
    M = glm::rotate(M, glm::radians(transform.rotation), {0,0,1});
    M = glm::scale(M, {transform.scale, 1});

    // 累积变换矩阵
    M = parent ? parent->GetModelMatrix() * M : M;

    return M;
}


glm::mat4 Node::GetModelMatrixRelativeTo(Node *target) {
    auto absTransMat = GetModelMatrix();
    auto newParentTransMat = target->GetModelMatrix();
    return glm::inverse(newParentTransMat) * absTransMat;
}

Transform Node::GetTransformByModelMatrix(const glm::mat4 &mat) {
    glm::vec3 newPosition, newScale, skew;
    glm::vec4 perspective;
    glm::quat newRotation;
    glm::decompose(mat, newScale, newRotation, newPosition, skew, perspective);

    glm::vec3 eulerAngleRadians = glm::eulerAngles(newRotation);
    glm::vec3 eulerAngleDegrees = glm::degrees(eulerAngleRadians);

    // 将新的变换数值赋值给s的transform
    Transform t;
    t.position = newPosition;
    t.scale = newScale;
    t.rotation = eulerAngleDegrees.z;
    return t;
}
