#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"

int main() {
    Window *window = Window::Get();
    window->Run();
    return 0;
}