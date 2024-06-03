#include "engine/script/LuaScriptLayer.hpp"
#include "engine/util/Print.hpp"
#include "engine/node/Node.hpp"
#include <sol/sol.hpp>
#include <fstream>

std::shared_ptr<class LuaScriptLayer> LuaScriptLayer::instance;

LuaScriptLayer::LuaScriptLayer() {
    lua.open_libraries(sol::lib::base);
}

std::shared_ptr<LuaScriptLayer> LuaScriptLayer::Get() {
    if (!instance) {
        instance = std::make_shared<LuaScriptLayer>();
    }
    return instance;
}

LuaScriptLayer::~LuaScriptLayer() {
}

void LuaScriptLayer::BindClass() {}

void LuaScriptLayer::ExecuteScript(const Script &script) {
    // 1. 读取
    std::string content;
    std::ifstream fin(script.GetFilePath());
    std::string text;
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        content = ss.str();
    } else {
        util::Println("Open Lua Script Failed: ", script.GetFilePath());
    }

    lua.script(content);

    lua.new_usertype<Node>("Node",
                                 sol::constructors<Node()>());
}

sol::state *LuaScriptLayer::GetLua(){
    return &lua;
}
