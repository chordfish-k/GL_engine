#include "engine/core/MainWindow.hpp"

int main() {
    MainWindow *window = MainWindow::Get();
    window->SetDefaultScenePath("test/scenes/b.scene");
    window->Run();
    return 0;
}