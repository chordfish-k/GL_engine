#pragma once

#include <string>
#include <sol/state.hpp>

class Script {
public:
    std::string filePath;
    sol::table self;
    bool attached = false;
public:
    Script() {};
    bool Imgui();
};