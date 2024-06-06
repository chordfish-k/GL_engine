#include "engine/script/LuaGlobalBinder.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/util/PrefabsUtils.hpp"
#include "engine/renderer/DebugDraw.hpp"

void LuaGlobalBinder::Bind(sol::state &state) {
    auto prefab = state.create_named_table("Prefab");
    prefab["CreateByFile"] = &PrefabsUtils::GenerateNodeFromFilePath;

    auto draw = state.create_named_table("Draw");
    draw["Line"] = [](glm::vec2 from, glm::vec2 to, glm::vec4 color={1,1,1,1}) {
        DebugDraw::AddLine2D(from, to, color);
    };


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