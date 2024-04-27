#include "engine/core/KeyListener.hpp"

KeyListener *KeyListener::instance = nullptr;

KeyListener *KeyListener::Get() {
    if (instance == nullptr) {
        instance = new KeyListener();
    }
    return instance;
}

void KeyListener::KeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    if (action == GLFW_PRESS) {
        Get()->keyPressed[key] = true;
        Get()->keyBeginPress[key] = true;
    } else if (action == GLFW_RELEASE) {
        Get()->keyPressed[key] = false; // Fixed here
        Get()->keyBeginPress[key] = false;
    }
}

bool KeyListener::IsKeyPressed(int keyCode) {
    if (keyCode < KEY_NUM) {
        return Get()->keyPressed[keyCode];
    }
    return false;
}
