#pragma once

#include <sol/state.hpp>
#include "IScriptLayer.hpp"

class LuaScriptLayer : public IScriptLayer {
public:
    LuaScriptLayer();
    ~LuaScriptLayer() override;
    static std::shared_ptr<LuaScriptLayer> Get();
    void BindClass() override;
    void ExecuteScript(const Script &script) override;
    sol::state *GetLua();
private:
    sol::state lua;
    static std::shared_ptr<LuaScriptLayer> instance;
};
