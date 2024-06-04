#pragma once

#include <sol/state.hpp>

class LuaNodeBinder {
public:
    static void Bind(sol::state &state);
};
