#pragma once

#include "Script.hpp"

class IScriptLayer {
public:
    virtual ~IScriptLayer() = default;
    virtual void BindClass() = 0;
    virtual void ExecuteScript(const Script& script) = 0;
};
