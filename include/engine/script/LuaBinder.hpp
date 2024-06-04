#pragma once

#include <sol/state.hpp>

class LuaBinder {
public:
    static void BindAll(sol::state& state);
};
