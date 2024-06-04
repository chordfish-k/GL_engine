#pragma once

#include <sol/state.hpp>

class LuaGlobalBinder {
public:
    static void Bind(sol::state& state);
};

