#pragma once

#include "engine/component/Component.hpp"

#include <string>
#include <type_traits>
#include <vector>

class GameObject {
private:
    std::string name;
    std::vector<Component *> components;

public:
    GameObject(std::string name);

    ~GameObject();

    template <typename T>
    std::enable_if_t<std::is_base_of<Component, T>::value, T *>
    GetComponent() {
        for (auto c : components) {
            if (!std::is_same_v<decltype(c), T>)
                continue;
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
    std::enable_if_t<std::is_base_of<Component, T>::value, T *>
    AddComponent() {
        T *comp = new T();
        components.push_back(comp);
        return comp;
    }

    void Update(float dt);

    void Start();
};
