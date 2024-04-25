#pragma once

#include "engine/component/Component.hpp"
#include "engine/component/Transform.hpp"
#include "engine/util/Print.hpp"

#include <string>
#include <type_traits>
#include <vector>

class GameObject {
private:
    std::string name;
    std::vector<Component *> components;
    int zIndex;

public:
    Transform *transform;

public:
    GameObject(std::string name);

    GameObject(std::string name, Transform *transform);

    GameObject(std::string name, Transform *transform, int zIndex);

    ~GameObject();

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

    // 属性
    int ZIndex() { return zIndex; }
};
