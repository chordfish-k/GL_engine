#pragma once
#include <filesystem>


namespace fs = std::filesystem;

class FileSystemWindow {
public:
    static fs::path localPath;
public:
    static void Imgui();

private:
    static void ShowPath();

    static void ShowFilesAndDirs();

    static std::string TruncateText(const std::string &text, float maxWidth);
};