#pragma once

#include "Window.hpp"

class ManagerWindow : public Window {
public:
    ManagerWindow();
    ~ManagerWindow() override = default;
    void Run() override;
    void Init() override;
    void Loop() override;
};
