﻿#pragma once

#include <rttr/type.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration>
#include "engine/core/ASerializableObj.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/Transform.hpp"
#include "engine/core/Window.hpp"
#include "engine/renderer/Renderer.hpp"

class GameObject;

class Node : public ASerializableObj{
private:
    static int ID_COUNTER;
    int uid = -1;

    int zIndex = 0;
    bool doSerialization = true;
    bool shouldDestroy = false;

public:
    Node *parent = nullptr;
    std::vector<Node *> children;
    Transform transform;
    const std::string nodeType = "Node";
    std::string name = nodeType;

public:
    Node() : Node(glm::vec2(), glm::vec2(1,1)){}

    Node(glm::vec2 position, glm::vec2 scale = glm::vec2(1,1))
        : transform(Transform(position, scale)) {}

    ~Node();

    virtual std::string GetNodeType() {return nodeType;}

    virtual void Start();

    virtual void Update(float dt);

    virtual void Imgui();

    virtual void Destroy() {}
    static void Init(int maxId) {ID_COUNTER = maxId; }

    bool IsDoSerialization() const { return doSerialization; }

    Transform GetTransform() { return parent ? parent->GetTransform() + transform : transform;}

    virtual std::string GetName() { return name; }


    template <typename T>
    std::enable_if_t<std::is_base_of<Node, T>::value, T *>
    AddNode() {
        T *comp = new T();
        AddNode(comp);
        return comp;
    }

    Node *AddNode(Node *comp);

    void RemoveAllNodes();


    virtual json Serialize();

    virtual Node *Deserialize(json j);

    void TravelOnSubTree(std::function<void(Node*)>);

    void GeneratedId();

    int GetUid() const { return uid; }

    int GetZIndex() const { return zIndex; }

    glm::mat4 Node::GetModelMatrix() ;

    Node *SetZIndex(int zIndex) {
        zIndex = zIndex;
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

protected:
    // TODO 补全所有类型
    template <typename T>
    std::enable_if_t<std::is_base_of<Node, T>::value, void>
    Imgui(){

        ShowTransformProperties();


        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Properties")) {

            auto a = rttr::type::get<T>();
            auto props = a.get_properties();

            if (!props.size()) return;

            auto &obj = *(T*)(this);
            ImGui::AlignTextToFramePadding();

            for (auto &p : props) {

                auto value = p.get_value(obj);

                if (!value)
                    continue;
                // int
                if (value.is_type<int>()) {
                    auto v = value.get_value<int>();
                    if (ImGui::DragInt(std::string(p.get_name()).c_str(), &v)) {
                        p.set_value(obj, v);
                    }
                }
                // int
                else if (value.is_type<float>()) {
                    auto v = value.get_value<float>();
                    if (ImGui::DragFloat(std::string(p.get_name()).c_str(),
                                         &v)) {
                        p.set_value(obj, v);
                    }
                }
                // bool
                else if (value.is_type<bool>()) {
                    auto v = value.get_value<bool>();
                    if (ImGui::Checkbox(std::string(p.get_name()).c_str(),
                                        &v)) {
                        p.set_value(obj, v);
                    }
                }
                // vec2
                else if (value.is_type<glm::vec2>()) {
                    auto v2 = value.get_value<glm::vec2>();
                    if (ImGui::DragFloat2(std::string(p.get_name()).c_str(),
                                          glm::value_ptr(v2))) {
                        p.set_value(obj, glm::vec2(v2.x, v2.y));
                    }
                }
                // vec3
                else if (value.is_type<glm::vec3>()) {
                    auto v3 = value.get_value<glm::vec3>();
                    if (ImGui::DragFloat3(std::string(p.get_name()).c_str(),
                                          glm::value_ptr(v3))) {
                        p.set_value(obj, glm::vec3(v3.x, v3.y, v3.z));
                    }
                }
                // vec4
                else if (value.is_type<glm::vec4>()) {
                    auto v4 = value.get_value<glm::vec4>();
                    if (ImGui::DragFloat4(std::string(p.get_name()).c_str(),
                                          glm::value_ptr(v4))) {
                        p.set_value(obj, glm::vec4(v4.x, v4.y, v4.z, v4.w));
                    }
                }
            }

            ImGui::TreePop();
        }
    }

private:
    void ShowTransformProperties();
};

#define COMPONENT(name_)                             \
    class name_ : public Node {                 \
        public:                                     \
            const std::string nodeType = #name_;     \
            std::string name = #name_;                                       \
            std::string GetNodeType() {return nodeType;}\
            std::string GetName() {return name;}
