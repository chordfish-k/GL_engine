#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace util {

template <typename... Args>
std::string Concat(const Args &...args) {
    std::stringstream ss;
    ((ss << args), ...);
    return ss.str();
}

template <typename... Args>
void Print(const Args &...args) {
    ((std::cout << args), ...);
}

template <typename... Args>
void Println(const Args &...args) {
    Print(args..., "\n");
}

inline void GLFWErrorCallBack(int code, const char *msg) {
    Print("[", code, "] ", msg);
}

} // namespace util
