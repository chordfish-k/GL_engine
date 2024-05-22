#include "engine/core/MainWindow.hpp"

int main() {
    MainWindow *window = MainWindow::Get();
    window->Run();
    return 0;
}