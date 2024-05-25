﻿#include "engine/core/MainWindow.hpp"
#include "engine/core/EditorSceneInitializer.hpp"

int main() {
    MainWindow *window = MainWindow::Get();
    window->SetDefaultScenePath("test/scenes/c.scene");
    window->Run();
    return 0;
}