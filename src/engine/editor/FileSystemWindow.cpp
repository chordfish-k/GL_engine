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

    const float height = ImGui::GetTextLineHeight()+4;

    ImGui::Text("/");
    ImGui::SameLine();
    std::string id = "pathBtn"+std::to_string(index);

    ImGui::PushID(id.c_str());
    if (ImGui::Button(piece.c_str(), ImVec2(0, height))) {
        localPath = fs::path(total);
    }
    ImGui::PopID();

    style = styleOrigin;


    // 添加文件夹按钮和添加文件按钮
    static bool readyToAdd = false;
    if (ImGui::Button("Add", ImVec2(0, height))) {
        readyToAdd = true;
    }
    if (readyToAdd) {
        static char buf[128] = {0};
        ImGui::PushItemWidth(150);
        ImGui::SameLine();
        ImGui::InputText("##input", buf, sizeof(buf));
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("As folder", ImVec2(0, height))) {
            std::string fileName = (fs::path(path) /  buf).string();
            try {
                fs::create_directory(fileName);
            } catch (const fs::filesystem_error& e) {
                util::Println("Error: ", e.what());
            }
            readyToAdd = false;
            buf[0] = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("As file", ImVec2(0, height))) {
            std::string fileName = (fs::path(path) /  buf).string();
            try {
                std::ofstream file(fileName);
                if (file.is_open()) {
                    file.close();
                }
            } catch (const std::ofstream::failure& e) {
                util::Println("Error: ", e.what());
            }
            readyToAdd = false;
            buf[0] = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(0, height))) {
            readyToAdd = false;
            buf[0] = 0;
        }
    }
}

void FileSystemWindow::ShowFilesAndDirs() {
    auto path = ProjectManagerWindow::projectLocation;
    if (!localPath.empty()) {
        fs::path p = fs::path(path) / localPath;
        path = p.string();
    }

    try {
        if (!fs::exists(path) || !fs::is_directory(path)) {
            return;
        }

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
                auto filePath = (fs::path(path)/name).string();
                // 如果是场景文件，则打开场景
                if (util::String::EndsWith(name, ".scene")) {
                    if (ImGui::Button(name.c_str(), size)) {
                        Window::ChangeScene(new EditorSceneInitializer(filePath));
                        util::Println("Open:", name);
                    }
                }
                // 如果是图像文件
                else if (util::String::CheckSuffix(name, ".png|.jpg")) {
                    auto tex = AssetPool::GetTexture(filePath);
                    auto texId = tex->GetId();
                    auto imageWidth = tex->GetWidth();
                    auto imageHeight = tex->GetHeight();

                    // 计算长宽比
                    float aspectRatio = imageWidth / imageHeight;

                    // 计算目标尺寸
                    float targetWidth, targetHeight;
                    if (size.x / size.y > aspectRatio)
                    {
                        // 按钮比较宽
                        targetHeight = size.y;
                        targetWidth = size.y * aspectRatio;
                    }
                    else
                    {
                        // 按钮比较高
                        targetWidth = size.x;
                        targetHeight = size.x / aspectRatio;
                    }

                    // 计算图像在按钮中的位置偏移，使其居中
                    ImVec2 offset((size.x - targetWidth) * 0.5f, (size.y - targetHeight) * 0.5f);

                    // 使用InvisibleButton来创建一个固定大小的按钮区域
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });        // 按钮常态
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.1f }); // 鼠标悬停
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.0f });  // 按钮单击
                    if (ImGui::Button(("##btn"+name).c_str(), size))
                    {
                        // 处理按钮点击事件
                        util::Println(name);

                        // TODO 拖拽到SpriteRenderer属性面板，替换Sprite
                    }
                    ImGui::PopStyleColor(3);

                    // 获取当前窗口绘制列表
                    ImDrawList* drawList = ImGui::GetWindowDrawList();

                    // 获取按钮的绝对位置
                    ImVec2 buttonPos = ImGui::GetItemRectMin();

                    // 计算图片绘制区域的左上角和右下角坐标
                    ImVec2 imageMin = ImVec2(buttonPos.x + offset.x, buttonPos.y + offset.y + targetHeight);
                    ImVec2 imageMax = ImVec2(buttonPos.x + offset.x + targetWidth, buttonPos.y + offset.y);

                    // 绘制图片
                    drawList->AddImage((void*)(intptr_t)texId, imageMin, imageMax);
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