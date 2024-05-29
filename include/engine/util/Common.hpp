#pragma once

#include <cstddef>
#include <malloc.h>
#include <string>
#include <fstream>
#include <sstream>
#include "Json.hpp"
#include "Print.hpp"

namespace util {

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

inline std::string LoadTextFromFile(const std::string &path) {
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

inline nlohmann::json Str2Json(const std::string &str) {
    nlohmann::json j;
    try{
        j = nlohmann::json::parse(str);
    }catch (std::exception &e) {
        util::Println("Convert String to Json Failed.", e.what());
    }
    return j;
}

inline nlohmann::json LoadJsonFromFile(const std::string &path) {
    return util::Str2Json(LoadTextFromFile(path));
}



} // namespace util
