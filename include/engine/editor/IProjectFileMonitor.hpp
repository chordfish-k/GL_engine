#pragma once

#include <functional>
class IProjectFileMonitor {
public:
    typedef std::function<void(const std::string&file, int actionType)> OnChangeCallback;

protected:
    OnChangeCallback onChange;

public:
    virtual void Update() = 0;
    virtual void SetOnChangeCallback(OnChangeCallback onChange) = 0;
};
