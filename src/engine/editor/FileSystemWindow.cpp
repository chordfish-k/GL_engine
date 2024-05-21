#include <imgui.h>
#include <string>
#include <sstream>
#include "engine/editor/ProjectManagerWindow.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/FileSystemWindow.hpp"
#include "engine/core/Window.hpp"
#include "engine/core/EditorSceneInitializer.hpp"
#include "engine/util/StringUtils.hpp"

fs::path FileSystemWindow::localPath;

void FileSystemWindow::Imgui() {
    ImGui::Begin("Files");

    ShowPath();
    ImGui::NewLine();
    ShowFilesAndDirs();

    ImGui::End();
}

void FileSystemWindow::ShowPath() {
    std::string path = ProjectManagerWindow::projectLocation;
    if (!localPath.empty()) {
        fs::path p = fs::path(path) / localPath;
        path = p.string();
    }
    path = AssetPool::GetAbsolutePath(path);
    if (path.empty()) {
        return;
    }

    std::stringstream ss2;

    char c;
    std::string piece, total;

    int index = 0;

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiStyle styleOrigin = ImGui::GetStyle();
    style.ItemSpacing = ImVec2(1.0f, 1.0f); // 设置项之间的间距

    ImGui::Text("...");
    ImGui::SameLine();

    for (char i : path) {
        c = i;
        if (c == '/' || c == '\\' || c == '\0') {
            ss2 >> piece;
            if (!total.empty())
                total += "\\";
            total += piece;


//            if (total == ProjectManagerWindow::projectLocation) {
                ImGui::Text("/");
                ImGui::SameLine();
                std::string id = "pathBtn"+std::to_string(index);

                ImGui::PushID(id.c_str());
                if (ImGui::Button(piece.c_str(), ImVec2(0, ImGui::GetTextLineHeight()+4))) {
                    localPath = fs::path(total);
                }
                ImGui::PopID();
                ImGui::SameLine();
//            }

            piece = "";
            ss2.clear();
        } else {
            ss2 << c;
        }
    }
    ss2 >> piece;
    if (!total.empty())
        total += "\\";
    total += piece;

    ImGui::Text("/");
    ImGui::SameLine();
    std::string id = "pathBtn"+std::to_string(index);

    ImGui::PushID(id.c_str());
    if (ImGui::Button(piece.c_str(), ImVec2(0, ImGui::GetTextLineHeight()+4))) {
        localPath = fs::path(total);
    }
    ImGui::PopID();
    ImGui::SameLine();

    style = styleOrigin;
}

void FileSystemWindow::ShowFilesAndDirs() {
    auto path = ProjectManagerWindow::projectLocation;
    if (!localPath.empty()) {
        fs::path p = fs::path(path) / localPath;
        path = p.string();
    }

    try {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        float windowX2 = windowPos.x + windowSize.x;

        // 遍历指定路径下的所有文件和文件夹
        int index = 0;
        int fileNum = 0;

        // 获取文件夹和文件总数
        auto fit = fs::directory_iterator(path);
        for (const auto& entry : fit) {
            fileNum++;
        }

        fit = fs::directory_iterator(path);
        for (const auto& entry : fit) {
            // 如果是文件夹
            std::string name = entry.path().filename().string();
            ImVec2 size = ImVec2(100, 100);
            std::string id = "fileBtn"+std::to_string(index);
            ImGui::PushID(id.c_str());

            if (fs::is_directory(entry)) {
                if (ImGui::Button(name.c_str(), size)) {
                    localPath /= name;
                }
            }
            // 如果是文件
            else if (fs::is_regular_file(entry)) {
                if (ImGui::Button(name.c_str(), size)) {
                    // 如果是场景，则打开场景
                    if (util::String::EndsWith(name, ".scene")) {
                        Window::ChangeScene(new EditorSceneInitializer(
                            (fs::path(path)/name).string()));
                        util::Println("Open:", name);
                    }
                }
            }
            ImGui::PopID();

            ImVec2 lastButtonPos = ImGui::GetItemRectMax();
            float lastButtonX2 = lastButtonPos.x;
            float nextButtonX2 = lastButtonX2 + itemSpacing.x + size.x;
            if (index + 1 < fileNum && nextButtonX2 < windowX2) {
                ImGui::SameLine();
            }
        }
    } catch (const std::exception& ex) {
        util::Println("Exception: ", ex.what());
    }
}