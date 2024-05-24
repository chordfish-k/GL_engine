#pragma once

#include <rttr/type.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration>
#include "engine/core/ASerializableObj.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/Transform.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/editor/MyImGui.hpp"
#include "engine/renderer/Color.hpp"
#include "engine/core/ZIndex.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/core/AGuiObj.hpp"

class Node : public ASerializableObj{
    RTTR_ENABLE()
private:
    static int ID_COUNTER;
    int uid = -1;

    bool doSerialization = true;
    bool isPickable = true;
    bool shouldDestroy = false;

protected:
    ZIndex zIndex = 0;

public:
    Node *parent = nullptr;
    std::list<Node *> children;
    Transform transform;
    const std::string nodeType = "Node";
    std::string name = "";

public:
    Node() : Node(glm::vec2(), glm::vec2(1,1)){}

    Node(glm::vec2 position, glm::vec2 scale = glm::vec2(1,1))
        : transform(Transform(position, scale)) {}

    virtual ~Node();

    virtual std::string GetNodeType() {return nodeType;}

    static void Init(int maxId) {ID_COUNTER = maxId; }

    virtual void Start();

    virtual void Update(float dt);

    void CheckDelete();

//    void Imgui() override;

    virtual void Destroy() {
        shouldDestroy = true;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of<Node, T>::value, T *>
    AddNode(const std::string &name_ = "") {
        T *comp = new T();
        comp->name = name_;
        AddNode(comp);
        return comp;
    }

    Node *AddNode(Node *comp);

    void RemoveAllNodes();

    virtual json Serialize();

    virtual Node *Deserialize(json j);

    void TravelOnSubTree(std::function<void(Node*)>);

    bool IsChildOf(Node *p);

    void GeneratedId(bool force=false);

    int GetUid() const { return uid; }

    int GetZIndex() const { return zIndex.GetZIndex(); }

    glm::mat4 GetModelMatrix();

    glm::mat4 GetModelMatrixRelativeTo(Node *target);

    Transform GetTransformByModelMatrix(const glm::mat4& mat);

    bool ShouldDestroy() const;

    bool IsPickable() const;

    void SetIsPickable(bool isPickable);

    bool IsDoSerialization() const { return doSerialization; }

    Node *SetDoSerialization(bool enable) { doSerialization = enable; return this; }

    Transform GetTransform() { return parent ? parent->GetTransform() + transform : transform;}

    virtual std::string GetName() { return name == "" ? GetNodeType() : name; }

    Node *SetName(const std::string &name) {
        this->name = name;
        return this;
    }

    Node *SetZIndex(int zIndex) {
        zIndex = zIndex >= 0 ? zIndex : 0;
        this->zIndex = zIndex <= 100 ? zIndex : 100;
        return this;
    }

    Node *SetPosition(glm::vec2 position) {
        transform.position = position;
        return this;
    }

    Node *SetScale(glm::vec2 scale) {
        transform.scale = scale;
        return this;
    }

    Node *SetRotation(float rotation) {
        transform.rotation = rotation;
        return this;
    }

    Node *SetTransform(Transform t) {
        transform = t;
        return this;
    }
public:
    virtual void Imgui() {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Node")) {
            transform.Imgui();
            zIndex.Imgui();
        }
    }

protected:
    void ShowImgui(std::vector<std::string> notShowFields = {}){

        auto &obj = *(this);

        auto thisType = rttr::type::get_by_name(GetNodeType());
        auto baseClasses = thisType.get_base_classes();

        //            auto thisType = rttr::type::get<T>();

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
        }
    }
};

#define COMPONENT(name_)                                \
    class name_ : public Node {                         \
    RTTR_ENABLE(Node)                                  \
    public:                                             \
        const std::string nodeType = #name_;            \
        std::string GetNodeType() {return nodeType;}

#define COMPONENT_E(name_, parent)                        \
    class name_ : public Parent {                       \
    RTTR_ENABLE(parent)                                \
    public:                                             \
        const std::string nodeType = #name_;            \
        std::string GetNodeType() {return nodeType;}

