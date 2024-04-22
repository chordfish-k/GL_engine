#include "engine/core/Window.hpp"

int main() {
    Window *window = Window::Get();
    window->SetWidth(800);
    window->SetHeight(600);
    window->Run();
    return 0;
}