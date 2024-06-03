#include <utility>

#include "engine/script/Script.hpp"
#include "engine/script/LuaScriptLayer.hpp"

Script::Script(std::string filePath)
    : filePath(std::move(filePath)){}

const std::string &Script::GetFilePath() const {
    return filePath;
}

void Script::Execute() const {
    LuaScriptLayer::Get()->ExecuteScript(*this);
}
