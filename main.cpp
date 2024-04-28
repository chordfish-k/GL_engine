#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"

int main() {
    Window *window = Window::Get();
    window->Run();
    return 0;
}