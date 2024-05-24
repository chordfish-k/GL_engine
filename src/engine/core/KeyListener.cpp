#include "engine/core/KeyListener.hpp"
#include "engine/util/Print.hpp"
#include "engine/event/EventSystem.hpp"

KeyListener *KeyListener::instance = nullptr;

KeyListener *KeyListener::Get() {
    if (instance == nullptr) {
        instance = new KeyListener();
    }
    return instance;
}

void KeyListener::EndFrame() {
    for (int i = 0; i < KEY_NUM; i++) {
        Get()->lastKeyPressed[i] = Get()->keyPressed[i];
    }
}

void KeyListener::KeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    if (action == GLFW_PRESS) {
        Get()->keyPressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        Get()->keyPressed[key] = false; // Fixed here
    }
}

bool KeyListener::IsKeyDown(int keyCode) {
    if (keyCode < KEY_NUM) {
        return Get()->keyPressed[keyCode];
    }
    return false;
}


bool KeyListener::IsKeyPressed(int keyCode) {
    if (keyCode < KEY_NUM) {
        return !Get()->lastKeyPressed[keyCode] && Get()->keyPressed[keyCode];
    } else {
        return false;
    }
}
void KeyListener::DoShortcutKeys() {
    // TODO 抽离注册逻辑
    if (KeyListener::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        if (KeyListener::IsKeyPressed(GLFW_KEY_S)) {
            EventSystem::Notify(nullptr, {EventType::SaveScene});
        }
        else if (KeyListener::IsKeyPressed(GLFW_KEY_L)) {
            EventSystem::Notify(nullptr, {EventType::LoadScene});
        }

    }
    else if (KeyListener::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        EventSystem::Notify(nullptr, {EventType::CloseTopWindow});
    }
}
