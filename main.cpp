#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/core/Window.hpp"

int main() {
    Window *window = Window::Get();
    window->SetWidth(1920 * 0.5);
    window->SetHeight(1080 * 0.5);
    window->Run();
    return 0;
}