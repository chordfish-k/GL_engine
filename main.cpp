#include "engine/core/MainWindow.hpp"
#include <sol/sol.hpp>



int main() {
    MainWindow *window = MainWindow::Get();
    window->SetDefaultScenePath("test/scene/a.scene");
    window->Run();
    return 0;
}