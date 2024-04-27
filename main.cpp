#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/core/Window.hpp"
#include "engine/util/Print.hpp"

int main() {
    Window::SetWidth(1920 * 0.5);
    Window::SetHeight(1080 * 0.5);
    Window::Run();
    return 0;
}