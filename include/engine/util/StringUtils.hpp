#pragma once
#include <string>

#include <vector>
#include <sstream>
#include <algorithm>

namespace util {

class String {
public:
    // 检查字符串是否以指定前缀开头
    static bool StartsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
               str.compare(0, prefix.size(), prefix) == 0;
    }

    // 检查字符串是否以指定后缀结尾
    static bool EndsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    // 按照指定的分隔符将字符串分割成多个部分
    static std::vector<std::string> Split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // 将字符串中的指定子串替换为另一个子串
    static std::string Replace(const std::string& str, const std::string& from, const std::string& to) {
        std::string result = str;
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        return result;
    }

    // 使用不定参数模板函数拼接多个字符串
    template<typename... Args>
    static std::string Concat(const Args&... args) {
        return (args + ...);
    }
};

}
