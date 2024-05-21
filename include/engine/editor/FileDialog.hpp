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
                                    const std::string &filter = "") {
        FileDialog::callback = callback;
        IGFD::FileDialogConfig config;
        config.path = ProjectManagerWindow::projectLocation;
        config.fileName = defaultPath;
        ImGuiFileDialog::Instance()->OpenDialog("ChooseDir", title,
                                                filter.empty() ? nullptr : filter.c_str(), config);
    }

    static void Imgui() {
        if (ImGuiFileDialog::Instance()->Display("ChooseDir")) {
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName =
                    ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath =
                    ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                callback(filePath, filePathName);
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }
};

