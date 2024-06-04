#pragma once

#include <sol/state.hpp>
#include "Script.hpp"
#include "engine/util/Print.hpp"
#include "engine/node/Node.hpp"

class LuaScriptManager {
public:
    void AddScriptNode(Node *node);

    virtual void OnGameStart();
    virtual void OnGameUpdate(float dt);

    template<typename ...Args>
    void Call(Script &script, const std::string& fnName, Args&& ...args) {
        sol::function_result result = script.self[fnName].call(std::forward<Args>(args)...);
        if(!result.valid())
        {
            sol::error err = result;
            util::Println(err.what());
        }
    }
private:
    sol::state state;
    std::vector<Node*> scripts;
};
