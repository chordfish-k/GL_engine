#pragma once
#include "engine/util/StringUtils.hpp"

namespace util {

    bool String::StartsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
               str.compare(0, prefix.size(), prefix) == 0;
    }

    // 检查字符串是否以指定后缀结尾
    bool String::EndsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    bool String::CheckSuffix(const std::string& str, const std::string& suffix_list) {
        auto suffix = Split(suffix_list, '|');
        for (const auto& suf : suffix) {
            if (str.size() >= suf.size() &&
                str.compare(str.size() - suf.size(), suf.size(), suf) == 0) {
                return true;
            }
        }
        return false;
    }

    // 按照指定的分隔符将字符串分割成多个部分
    std::vector<std::string> String::Split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // 按照指定的字符串分隔符将字符串分割成多个部分
    std::vector<std::string> String::Split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }

    // 将字符串中的指定子串替换为另一个子串
    std::string String::Replace(const std::string& str, const std::string& from, const std::string& to) {
        std::string result = str;
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        return result;
    }

    std::string String::GetSuffix(const std::string &str, char c) {
        // 找到字符串中最后一个字符c的位置
        size_t lastPos = str.find_last_of(c);

        // 如果未找到字符c，返回空字符串
        if (lastPos == std::string::npos) {
            return "";
        }

        // 返回最后一个字符c后面的字符串
        return str.substr(lastPos + 1);
    }

    // 将字符串的首字母转换为大写
    std::string String::CapitalizeFirst(const std::string& str) {
        if (str.empty()) {
            return str;
        }

        std::string result = str;
        result[0] = std::toupper(result[0]);
        return result;
    }

    // 将字符串的首字母转换为小写
    std::string String::LowercaseFirst(const std::string& str) {
        if (str.empty()) {
            return str;
        }

        std::string result = str;
        result[0] = std::tolower(result[0]);
        return result;
    }

    std::string String::TBS(const std::string &str) {
        typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
        static std::wstring_convert<F> strC(new F("Chinese"));
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> strCnv;
        return strCnv.to_bytes(strC.from_bytes(str));
    }
}
