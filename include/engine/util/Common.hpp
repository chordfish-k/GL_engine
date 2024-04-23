#pragma once

#include <cstddef>
#include <string>

namespace util {

template <typename T, size_t N>
size_t LenOf(T (&arr)[N]) {
    return N;
}

inline std::string Trim(std::string str) {
    size_t size = str.size();
    size_t begin = 0;
    size_t end = size - 1;
    while (begin < size && (str[begin] == ' ' || str[begin] == '\n'))
        begin++;

    while (end >= 0 && (str[end] == ' ' || str[end] == '\n'))
        end--;

    return str.substr(begin, end - begin + 1);
}

} // namespace util
