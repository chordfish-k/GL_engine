#include "engine/script/LuaGlobalBinder.hpp"
#include "engine/core/KeyListener.hpp"

void LuaGlobalBinder::Bind(sol::state &state) {
    auto input = state.create_named_table("Input");
    input["IsKeyDown"] = &KeyListener::IsKeyDown;
    input["IsKeyPressed"] = &KeyListener::IsKeyPressed;

    auto keys = state.create_named_table("Keys");
    keys["a"] = GLFW_KEY_A;
    keys["d"] = GLFW_KEY_D;
    keys["w"] = GLFW_KEY_W;
    keys["s"] = GLFW_KEY_S;
    keys["space"] = GLFW_KEY_SPACE;
}