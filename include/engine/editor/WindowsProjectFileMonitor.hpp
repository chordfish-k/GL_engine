#pragma once

#include "IProjectFileMonitor.hpp"
#include <Windows.h>
#include <thread>

class WindowsProjectFileMonitor : public IProjectFileMonitor{
private:
    static WindowsProjectFileMonitor *instance;
    std::string projectRootPath;

    HANDLE hFile = reinterpret_cast<HANDLE>(-1);
    std::thread monitorThread;
    bool threadRunning = false;
    OVERLAPPED overlapped = { 0 };

private:
    WindowsProjectFileMonitor();

    WindowsProjectFileMonitor(const std::string &projectRootPath);

public:
    static WindowsProjectFileMonitor *Get();

    void Init(const std::string &projectRootPath);

    void Update() override;

    void Stop();

    void SetOnChangeCallback(
        std::function<void(const std::string &, int)> function) override;
};
