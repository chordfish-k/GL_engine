#pragma once

#include <rttr/type.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration>
#include "engine/core/ASerializableObj.hpp"
#include "engine/util/Print.hpp"

class GameObject;

class Component : public ASerializableObj{
private:
    static int ID_COUNTER;
    int uid = -1;

public:
    GameObject *gameObject = nullptr;
    const std::string componentName = "Component";

public:
    Component() {};
    virtual std::string GetComponentName() {return componentName;}
    virtual void Start() {};
    virtual void Update(float dt) {};
    virtual void Imgui() {};
    virtual void Destroy() {}
    static void Init(int maxId) {ID_COUNTER = maxId; }

    virtual json Serialize() {
        json j;
        j["component"] = GetComponentName();
        return j;
    };
    virtual ASerializableObj *Deserialize(json j) {return nullptr;}

    void GeneratedId() {
        if (this->uid == -1) {
            this->uid = ID_COUNTER++;
        }
    }

    int GetUid() const { return uid; }

protected:
    // TODO 补全所有类型
    template <typename T>
    std::enable_if_t<std::is_base_of<Component, T>::value, void>
    Imgui(){
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(((T*)(this))->componentName.c_str())) {

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
};

#define COMPONENT(name)                             \
    class name : public Component {                 \
        public:                                     \
            const std::string componentName = #name;\
            std::string GetComponentName() {return componentName;}

