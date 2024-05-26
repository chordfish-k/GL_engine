#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MainWindow.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

int Node::ID_COUNTER = 0;

Node::Node (glm::vec2 position, glm::vec2 scale) {}

Node::~Node(){
    Delete();
}

void Node::Delete() {
    auto renderer = MainWindow::GetScene()->GetRenderer();
    auto physics2D = MainWindow::GetScene()->GetPhysics2D();
    renderer->DestroyNode(this);
    physics2D->DestroyNode(this);
    for (auto &c : children) {
        delete c;
        c = nullptr;
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

void Node::EditorUpdate(float dt) {
    for (auto go : children) {
        go->EditorUpdate(dt);
    }
}

void Node::CheckDelete() {
    auto &ch = children;

    for (auto it = ch.begin(); it != ch.end();) {
        auto go = (*it);
        go->CheckDelete();

        if (go->ShouldDestroy()) {
            it = ch.erase(it);
            go->Delete();
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
            auto &ty = ch["type"];
            if (ty.empty())
                continue;
            std::string type = ty;

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

void Node::Imgui() {
    auto name_ = GetName();
    if (MyImGui::DrawTextInput("name", name_)){
        name = name_;
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Node")) {
        transform.Imgui();
        zIndex.Imgui();
    }
}


void Node::ShowImgui(std::vector<std::string> notShowFields){
    auto &obj = *(this);

    auto thisType = rttr::type::get_by_name(GetNodeType());
    auto baseClasses = thisType.get_base_classes();

    auto props = thisType.get_properties();

    if (!props.size()) return;

    ImGui::AlignTextToFramePadding();

    for (auto &p : props) {
        if (0 < std::count(notShowFields.begin(), notShowFields.end(), p.get_name()))
            continue;
        rttr::variant value = p.get_value(obj);
        auto declaringType = p.get_declaring_type();

        // 只显示自身定义的，不显示父类的
        if (declaringType != thisType || !value)
            continue;
        // int
        if (value.is_type<int>()) {
            auto v = value.get_value<int>();
            if (MyImGui::DrawIntControl(std::string(p.get_name()), v)) {
                p.set_value(obj, v);
            }
        }
        // float
        else if (value.is_type<float>()) {
            auto v = value.get_value<float>();
            if (MyImGui::DrawFloatControl(std::string(p.get_name()),v)) {
                p.set_value(obj, v);
            }
        }
        // bool
        else if (value.is_type<bool>()) {
            auto v = value.get_value<bool>();
            if (MyImGui::DrawCheckbox(std::string(p.get_name()),v)) {
                p.set_value(obj, v);
            }
        }
        // vec2
        else if (value.is_type<glm::vec2>()) {
            auto v2 = value.get_value<glm::vec2>();
            if (MyImGui::DrawVec2Control(std::string(p.get_name()), v2)) {
                p.set_value(obj, v2);
            }
        }
        // vec3
        else if (value.is_type<glm::vec3>()) {
            auto v3 = value.get_value<glm::vec3>();
            if (MyImGui::DrawVec3Control(std::string(p.get_name()),v3)) {
                p.set_value(obj, v3);
            }
        }
        // vec4
        else if (value.is_type<glm::vec4>()) {
            auto v4 = value.get_value<glm::vec4>();
            if (MyImGui::DrawVec4Control(std::string(p.get_name()),
                                         v4)) {
                p.set_value(obj, v4);
            }
        }

        else if (value.is_type<Color>()) {
            auto v4 = value.get_value<Color>().ToVec4();
            if (MyImGui::DrawColor4Control(std::string(p.get_name()),
                                           v4)) {
                p.set_value(obj, Color(v4.x, v4.y, v4.z, v4.w));
            }
        }
        else if (value.is_type<BodyType>()) {
            std::vector<std::string> enum_names;
            std::vector<rttr::variant> enum_values;
            auto type = rttr::type::get<BodyType>();
            auto enumerators = type.get_enumeration().get_names();
            int index = 0, currentIndex = 0;
            BodyType now = value.get_value<BodyType>();

            for (const auto&n : enumerators){
                enum_names.emplace_back(n.data());
                auto v = type.get_enumeration().name_to_value(n);
                enum_values.push_back(v);
                if (v == rttr::variant(now)) {
                    index = currentIndex;
                }
                ++currentIndex;
            }
            // 使用 enum_names 的数据绘制 ImGui::Combo
            if (ImGui::Combo("bodyType", &index, [](void* data, int idx, const char** out_text)
                 {
                     auto* names = static_cast<const std::vector<std::string>*>(data);
                     if (idx < 0 || idx >= names->size())
                         return false;
                     *out_text = (*names)[idx].c_str();
                     return true;
                 }, static_cast<void*>(&enum_names), enum_names.size()))
            {
                BodyType selected_value = enum_values[index].get_value<BodyType>();
                p.set_value(obj, selected_value);
            }
        }
    }
}