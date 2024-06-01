#pragma once
#include <string>

#include <vector>
#include <sstream>
#include <algorithm>
#include <codecvt>

namespace util {

class String {
public:
    // 检查字符串是否以指定前缀开头
    static bool StartsWith(const std::string &str, const std::string &prefix);

    // 检查字符串是否以指定后缀结尾
    static bool EndsWith(const std::string &str, const std::string &suffix);

    static bool CheckSuffix(const std::string &str,
                            const std::string &suffix_list);

    // 按照指定的分隔符将字符串分割成多个部分
    static std::vector<std::string> Split(const std::string &str,
                                          char delimiter);

    // 按照指定的字符串分隔符将字符串分割成多个部分
    static std::vector<std::string> Split(const std::string &str,
                                          const std::string &delimiter);

    // 将字符串中的指定子串替换为另一个子串
    static std::string Replace(const std::string &str, const std::string &from,
                               const std::string &to);

    // 辅助函数，提供简便接口
    template<typename... Args>
    std::string Concat(Args&&... args) {
        std::stringstream ss;
        appendToStream(ss, std::forward<Args>(args)...);
        return ss.str();
    }

    static std::string GetSuffix(const std::string &str, char c);

    // 将字符串的首字母转换为大写
    static std::string CapitalizeFirst(const std::string &str);

    // 将字符串的首字母转换为小写
    static std::string LowercaseFirst(const std::string &str);

    static std::string TBS(const std::string &str);

private:
    // 可变参数模板函数，用于递归拼接字符串
    template<typename T, typename... Args>
    static void AppendToStream(std::stringstream& ss, T&& first, Args&&... rest) {
        ss << std::forward<T>(first);
        appendToStream(ss, std::forward<Args>(rest)...);
    }

    // 基础模板函数，用于终止递归
    static void AppendToStream(std::stringstream& ss) {
        // 终止条件：不做任何操作
    }
};

}