#pragma once

#include <cstddef>
#include <malloc.h>
#include <string>

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

} // namespace util
