#pragma once

#include "AbstractScene.hpp"
#include "Window.hpp"
#include "engine/util/Print.hpp"
#include <GLFW/glfw3.h>

class LevelScene : public AbstractScene {
private:
    bool changingScene = false;
    float timeToChangeScene = 2.f;

public:
    LevelScene() {
        util::Println("Inside level scene.");
        Window::Get()->r = 1;
        Window::Get()->g = 1;
        Window::Get()->b = 1;
    }

    void Update(float dt) {}

    void Init() {}
};