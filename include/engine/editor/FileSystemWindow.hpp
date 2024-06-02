#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class FileSystemWindow {
public:
    static fs::path localPath;
    static std::vector<std::string> cache;

    static void SetLocalPath(const std::filesystem::path &path1);

public:
    static void Start();
    static void Imgui();

private:
    static void ShowPath();

    static void ShowFilesAndDirs();

    static void NodePrefabDragDrop();

    static void RefreshCache();

    static std::string TruncateText(const std::string &text, float maxWidth);
};
