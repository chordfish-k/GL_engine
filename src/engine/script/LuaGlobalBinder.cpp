#include "engine/script/LuaGlobalBinder.hpp"
#include "engine/core/KeyListener.hpp"

void LuaGlobalBinder::Bind(sol::state &state) {
    auto input = state.create_named_table("Input");
    input["IsKeyDown"] = &KeyListener::IsKeyDown;

    auto keys = state.create_named_table("Keys");
    keys["a"] = GLFW_KEY_A;
    keys["d"] = GLFW_KEY_D;
}