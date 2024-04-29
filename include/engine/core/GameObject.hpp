#pragma once

#include "engine/component/Component.hpp"
#include "engine/component/Transform.hpp"
#include "engine/util/Print.hpp"

#include <string>
#include <type_traits>
#include <vector>

class GameObject : public ASerializableObj{
private:
    std::vector<Component *> components;
    static int ID_COUNTER;
    int uid = -1;
    bool doSerialization = true;
    bool isDead = false;

public:
    std::string name;
    Transform *transform;

    int zIndex;

public:
    GameObject() {this->name = "Object";}

    GameObject(std::string name);

    GameObject(std::string name, Transform *transform);

    GameObject(std::string name, Transform *transform, int zIndex);

    ~GameObject();

    void RemoveAllComponent();

    void Destroy();

    static void init(int maxId);

    template <typename T>
    std::enable_if_t<std::is_base_of<Component, T>::value, T *> GetComponent() {
        for (auto c : components) {
            T *res = dynamic_cast<T *>(c);
            if (res) {
                return res;
            }
        }
        return nullptr;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of<Component, T>::value, T *>
    RemoveComponent() {
        for (auto it = components.begin(); it != components.end(); it++) {
            if (!std::is_same_v<decltype(it), T>)
                continue;
            T *res = dynamic_cast<T *>(it);
            if (res) {
                components.erase(it);
                return res;
            }
        }
        return nullptr;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of<Component, T>::value, T *> AddComponent() {
        T *comp = new T();
        AddComponent(comp);
        return comp;
    }

    Component *AddComponent(Component *comp) {
        components.push_back(comp);
        comp->gameObject = this;
        return comp;
    }

    void Update(float dt);

    void Start();

    void Imgui();

    // 属性
    int ZIndex() { return zIndex; }

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    int GetUid() const;

    bool IsDead() const;

    bool IsDoSerialization() const;

    void SetNoSerialization();

    void GenerateUid();
};