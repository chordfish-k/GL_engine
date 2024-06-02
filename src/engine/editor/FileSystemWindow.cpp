#include <imgui.h>
#include <string>
#include <sstream>
#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"
#include "engine/editor/FileSystemWindow.hpp"
#include "engine/core/EditorSceneInitializer.hpp"
#include "engine/util/StringUtils.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/util/Setting.hpp"
#ifdef _WIN32
#include <windows.h>
#include "engine/editor/WindowsProjectFileMonitor.hpp"
#endif

fs::path FileSystemWindow::localPath;
std::vector<std::string> FileSystemWindow::cache;

void FileSystemWindow::Start() {
    RefreshCache();
}

void FileSystemWindow::Imgui() {
    ImGui::Begin("Files");

    WindowsProjectFileMonitor::Get()->SetOnChangeCallback([](auto s, auto a){
        RefreshCache();
    });

    ShowPath();
    ShowFilesAndDirs();
    NodePrefabDragDrop();

    ImGui::End();
}

void FileSystemWindow::ShowPath() {
    std::string path = Setting::PROJECT_ROOT;
    if (!localPath.empty()) {
        fs::path p = fs::path(path) / localPath;
        path = p.string();
    }
    path = AssetPool::GetAbsolutePath(path);
    if (path.empty()) {
        return;
    }

    std::stringstream ss2;
    std::string total;

    int index = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 5.0f)); // 设置项之间的间距

    auto tokens = util::String::Split(util::String::Replace(localPath.string(), "/", "\\"), '\\');
    float height = ImGui::GetTextLineHeight() + 4;

    ImGui::PushID("~");

    if (ImGui::Button("~", ImVec2(0, height))) {
        localPath = fs::path(total);
        RefreshCache();
    }
    ImGui::PopID();


    for (std::string &token: tokens) {
        ImGui::SameLine();
        ImGui::Text("/");
        if (index > 0) total += '\\';

        std::string id = "pathBtn"+std::to_string(index++);
        total += token;

        ImGui::PushID(id.c_str());
        ImGui::SameLine();
        if (ImGui::Button(token.c_str(), ImVec2(0, height))) {
            localPath = fs::path(total);
            RefreshCache();
        }
        ImGui::PopID();
    }

    ImGui::PopStyleVar();

    // 添加文件夹按钮和添加文件按钮
    static bool readyToAdd = false;
    static char buf[128] = {0};
    if (!readyToAdd && ImGui::Button("Add", ImVec2(0, height))) {
        readyToAdd = true;
        buf[0] = 0;
    }
    if (readyToAdd && ImGui::Button("Cancel", ImVec2(0, height))) {
        readyToAdd = false;
    }
    if (readyToAdd) {

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
        }
    }
#ifdef _WIN32
    ImGui::SameLine();
    if (ImGui::Button("Open In Explorer", ImVec2(0, height))) {
        // 使用ShellExecute打开指定文件夹
        ShellExecute(NULL, "open", "explorer.exe", path.c_str(), NULL, SW_SHOWNORMAL);
    }
#endif
}

void FileSystemWindow::ShowFilesAndDirs() {
    auto path = Setting::PROJECT_ROOT;
    if (!localPath.empty()) {
        fs::path p = fs::path(path) / localPath;
        path = p.string();
    }
    if (!fs::exists(path) || !fs::is_directory(path)) {
        return;
    }

    ImGui::BeginChild("ScrollingFileIcon", ImVec2(0, 0), false, 0);


    try {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        float windowX2 = windowPos.x + windowSize.x;

        // 遍历指定路径下的所有文件和文件夹
        int index = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, ImGui::GetTextLineHeight()+8));
        auto cacheCopy = FileSystemWindow::cache;
        for (const auto& name : cacheCopy) {
            auto entry = Setting::PROJECT_ROOT / localPath / fs::path(name);
            std::string u8Name = util::String::TBS(name);

            ImVec2 btnSize = ImVec2(100, 100);
            std::string id = "fileBtn:"+name;

            ImGui::PushID(id.c_str());
            // 如果是文件夹
            if (fs::is_directory(entry)) {
                if (ImGui::Button("Dir", btnSize)) {
                    localPath /= name;
                    RefreshCache();
                }
            }
            // 如果是文件
            else {
                auto filePath = (fs::path(path)/name).string();
                // 如果是场景文件，则打开场景
                if (util::String::EndsWith(name, ".scene")) {
                    if (ImGui::Button("Scene", btnSize)) {
                        MainWindow::ChangeScene(filePath);
                    }
                }
                // 如果是预制体
                else if (util::String::CheckSuffix(name, ".pfb")) {
                    if (ImGui::Button("Pfb", btnSize)) {
                        // 将其作为场景打开
                        MainWindow::ChangeScene(filePath);
                    }
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        // 将字符串转换为字节数组
                        auto lPath =(Setting::PROJECT_ROOT / localPath / u8Name).string();
                        const char* textData = lPath.c_str();
                        size_t dataSize = (lPath.length() + 1) * sizeof(char); // 包括空字符 '\0'

                        // 设置拖放负载
                        ImGui::SetDragDropPayload("PREFAB", textData, dataSize);
                        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300.0f);
                        ImGui::TextWrapped("%s", u8Name.c_str());
                        ImGui::PopTextWrapPos();
                        ImGui::EndDragDropSource();
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
                    if (btnSize.x / btnSize.y > aspectRatio){
                        // 按钮比较宽
                        targetHeight = btnSize.y;
                        targetWidth = btnSize.y * aspectRatio;
                    }
                    else{
                        // 按钮比较高
                        targetWidth = btnSize.x;
                        targetHeight = btnSize.x / aspectRatio;
                    }

                    // 计算图像在按钮中的位置偏移，使其居中
                    ImVec2 offset((btnSize.x - targetWidth) * 0.5f, (btnSize.y - targetHeight) * 0.5f);

                    // 使用InvisibleButton来创建一个固定大小的按钮区域
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });        // 按钮常态
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.1f }); // 鼠标悬停
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.0f });  // 按钮单击

                    if (ImGui::Button(("##"+name).c_str(), btnSize)){
                        // 处理按钮点击事件
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

                    // 拖放源：给属性窗口赋值文件路径
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        // 将字符串转换为字节数组
                        auto lPath =(Setting::PROJECT_ROOT / localPath / u8Name).string();
                        const char* textData = lPath.c_str();
                        size_t dataSize = (lPath.length() + 1) * sizeof(char); // 包括空字符 '\0'

                        // 设置拖放负载
                        ImGui::SetDragDropPayload("FILE_PATH", textData, dataSize);
                        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300.0f);
                        ImGui::TextWrapped("%s", u8Name.c_str());
                        ImGui::PopTextWrapPos();
                        ImGui::EndDragDropSource();
                    }
                }
                // 其它文件
                else {
                    std::string suffix = util::String::CapitalizeFirst(util::String::GetSuffix(name, '.'));
                    suffix = suffix.empty() ? "?" : suffix;
                    if (ImGui::Button(suffix.c_str(), btnSize)) {
                    }
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", u8Name.c_str());
            ImGui::PopID();

            // 获取当前窗口绘制列表
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            // 获取按钮的绝对位置
            ImVec2 buttonPos = ImGui::GetItemRectMin();

            // 缩略过长文件名
            std::string fileNameStr = TruncateText(u8Name, btnSize.x-5);

            // 计算图片绘制区域的左上角和右下角坐标
            ImVec2 textSize = ImGui::CalcTextSize(fileNameStr.c_str());
            ImVec2 textPos = ImVec2(buttonPos.x + (btnSize.x - textSize.x) * 0.5, buttonPos.y + btnSize.y + 2); // 偏移5像素在按钮下方绘制
            // 绘制文本
            drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), fileNameStr.c_str());

            // 判断换行显示
            ImVec2 lastButtonPos = ImGui::GetItemRectMax();
            float lastButtonX2 = lastButtonPos.x;
            float nextButtonX2 = lastButtonX2 + itemSpacing.x + btnSize.x;
            if (index++ < cache.size() && nextButtonX2 < windowX2) {
                ImGui::SameLine();
            }
        }
        ImGui::PopStyleVar();
    }
    catch (const std::exception& ex) {
        util::Println("Exception: ", ex.what());
//        ImGui::EndChild();
    }
    ImGui::NewLine();
    ImGui::Dummy({0, ImGui::GetTextLineHeight()});
    ImGui::EndChild();
}

void FileSystemWindow::NodePrefabDragDrop() {
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Node*));
            Node *source = *((Node **)payload->Data);

            // 以pref文件保存json，统一将position设置成0
            Transform temp = source->transform;
            source->transform.position = {0, 0};
            auto jsonText = source->Serialize().dump(2);
            source->transform = temp;
            // TODO 弹出提示窗表示文件已存在，是否替换
            std::ofstream ofs( Setting::PROJECT_ROOT / localPath / (source->GetName() + ".pfb"),
                              std::ios::trunc);
            if (ofs.is_open()) {
                ofs << jsonText;
            }
            ofs.close();
        }
        ImGui::EndDragDropTarget();
    }
}

std::string FileSystemWindow::TruncateText(const std::string& text, float maxWidth)
{
    std::string truncatedText = text;
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    if (textSize.x <= maxWidth)
    {
        return text; // Text fits within maxWidth, no truncation needed
    }

    const char* ellipsis = "...";
    float ellipsisWidth = ImGui::CalcTextSize(ellipsis).x;

    while (!truncatedText.empty() && textSize.x + ellipsisWidth > maxWidth)
    {
        truncatedText.pop_back();
        textSize = ImGui::CalcTextSize(truncatedText.c_str());
    }

    return truncatedText + ellipsis;
}

void FileSystemWindow::RefreshCache() {
    fs::path p = Setting::PROJECT_ROOT / FileSystemWindow::localPath;
    FileSystemWindow::cache.clear();
    for (const auto &entry : fs::directory_iterator(p)) {
        FileSystemWindow::cache.push_back(entry.path().filename().string());
    }
}

void FileSystemWindow::SetLocalPath(const std::filesystem::path &path1) {
    localPath = path1;
    RefreshCache();
}
