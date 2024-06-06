#pragma once

#include <cstddef>
#include <malloc.h>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Json.hpp"
#include "Print.hpp"
#include "Setting.hpp"

namespace util {

namespace fs = std::filesystem;

template <typename T, size_t N>
size_t LenOf(T (&arr)[N]) {
    return N;
}

template <typename T>
size_t LenOfP(T *arr) {
    return _msize(arr) / sizeof(T);
}

inline std::string Trim(std::string str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos) {
        // 字符串全由空格字符组成
        return "";
    } else {
        return str.substr(first, last - first + 1);
    }
}



inline nlohmann::json Str2Json(const std::string &str) {
    nlohmann::json j;
    try{
        j = nlohmann::json::parse(str);
    }catch (std::exception &e) {
        util::Println("Convert String to Json Failed.", e.what());
    }
    return j;
}

inline std::string PathRelativeToProjectRoot(const fs::path &path) {
    auto pr = fs::path(Setting::PROJECT_ROOT);
    return relative(path, pr).string();
}

inline std::string GetRelativePathTo(const fs::path &path, const fs::path &base) {
    return relative(path, base).string();
}

inline std::string GetAbsolutePath(std::string path) {
//    fs::path curath = fs::current_path();
    fs::path curath = Setting::PROJECT_ROOT;
    fs::path abPath = curath / path;
    return abPath.string();
}

inline std::string LoadTextFromFile(const std::string &path) {
    if (path.empty()) return "";
    std::ifstream fin(path);
    std::string text;
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        text = ss.str();
    } else {
        util::Println("Open File Failed: ", path);
    }
    return text;
}

inline nlohmann::json LoadJsonFromFile(const std::string &path) {
    return util::Str2Json(LoadTextFromFile(path));
}

inline std::string Dump(const nlohmann::json& j, int indent = 2, int maxLineLen = 150) {
    // 导出json到字符串，其中数组不到最大长度不换行
    std::stringstream out;
    std::function<void(const nlohmann::json &, int, int &)> dump_helper =
        [&](const nlohmann::json &j, int current_indent,
            int &current_line_length) {
            if (j.is_object()) {
                out << "{\n";
                current_line_length = current_indent + indent;
                bool first = true;
                for (auto it = j.begin(); it != j.end(); ++it) {
                    if (!first) {
                        out << ",\n";
                        current_line_length = current_indent + indent;
                    }
                    out << std::string(current_indent + indent, ' ') << "\"" << it.key() << "\": ";
                    current_line_length += it.key().length() + 4; // Account for key and surrounding quotes/colon/space
                    dump_helper(it.value(), current_indent + indent, current_line_length);
                    first = false;
                }
                out << "\n" << std::string(current_indent, ' ') << "}";
                current_line_length = current_indent;
            } else if (j.is_array()) {
                out << "[";
                out << "\n" << std::string(current_indent + indent, ' ');
                current_line_length = current_indent + indent;
                for (size_t i = 0; i < j.size(); ++i) {
                    if (i != 0) {
                        out << ", ";
                        current_line_length += 2;
                    }
                    int element_length = j[i].dump().length();
                    if (current_line_length + element_length > maxLineLen) {
                        out << "\n" << std::string(current_indent + indent, ' ');
                        current_line_length = current_indent + indent;
                    }
                    dump_helper(j[i], current_indent + indent, current_line_length);
                    current_line_length += element_length;
                }
                out << "\n" << std::string(current_indent, ' ') << "]";
                current_line_length = current_indent;
            } else {
                std::string value = j.dump();
                out << value;
                current_line_length += value.length();
            }
        };

        int current_line_length = 0;
        dump_helper(j, 0, current_line_length);
        out << std::endl;
        return out.str();
    }
} // namespace util
