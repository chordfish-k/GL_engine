#include <string>
#include <sol/sol.hpp>
#include "engine/script/LuaScriptManager.hpp"
#include "engine/script/LuaBinder.hpp"
#include "engine/util/Common.hpp"

void LuaScriptManager::OnGameStart() {
    LuaBinder::BindAll(state);

    for (auto node : scripts){
        Script &item = node->script;
        if (item.filePath.empty()) continue;
        auto result = state.safe_script_file(
            util::GetAbsolutePath(item.filePath),
            &sol::script_pass_on_error);
        if(result.valid())
        {
            if(result.return_count() == 1 && result[0].is<sol::table>())
            {
               item.self = result[0];
               item.self["this"] = node;
               Call(item, "Start");
            }
        }
        else {
            sol::error err = result;
            util::Println(err.what());
        }
   }
}

void LuaScriptManager::OnGameUpdate(float dt) {
    for (auto node : scripts){
        Script &item = node->script;
        if (item.filePath.empty()) continue;
        Call(item, "Update",
             "dt", dt);
    }
}


void LuaScriptManager::AddScriptNode(Node *node) {
    if (scripts.end() == std::find(scripts.begin(), scripts.end(), node)) {
        scripts.push_back(node);
    }
}
