#include "engine/core/Window.hpp"

int main() {
    Window *window = Window::Get();
    window->Run();
    return 0;
}