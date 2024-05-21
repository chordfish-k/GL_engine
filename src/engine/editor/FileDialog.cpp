#include "engine/editor/FileDialog.hpp"
#include "engine/core/Window.hpp"

FileDialogCallback FileDialog::callback;

void FileDialog::DisplayWithCallback(const std::string &title, FileDialogCallback callback,
                                const std::string &defaultPath,
                                const std::string &filter) {
    FileDialog::callback = callback;
    IGFD::FileDialogConfig config;
    config.path = ProjectManagerWindow::projectLocation;
    config.fileName = defaultPath;
    config.flags |= ImGuiFileDialogFlags_Modal;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseDir", title,
                                            filter.empty() ? nullptr : filter.c_str(), config);
//    Window::SetShowingTopWindow(true);

}

void FileDialog::Imgui() {
    if (ImGuiFileDialog::Instance()->Display("ChooseDir")) {

        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
            callback(filePath, filePathName);
//            Window::SetShowingTopWindow(false);

        }


        // close
        ImGuiFileDialog::Instance()->Close();
    }
}