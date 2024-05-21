#pragma once

#include <string>
#include "ImGuiFileDialog.h"
#include "ProjectManagerWindow.hpp"

typedef std::function<void(const std::string&,const std::string&)> FileDialogCallback;

class FileDialog {
private:
    static FileDialogCallback callback;

public:
    static void DisplayWithCallback(const std::string &title, FileDialogCallback callback,
                                    const std::string &defaultPath = "",
                                    const std::string &filter = "");

    static void Imgui();
};

