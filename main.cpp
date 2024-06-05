#include "engine/core/MainWindow.hpp"
#include "engine/util/Common.hpp"


int main() {
    MainWindow *window = MainWindow::Get();
    window->SetDefaultScenePath("_project/scene/a.scene");
    window->Run();
    return 0;
}