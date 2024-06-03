#pragma once

#include <string>

class Script {
private:
    std::string filePath;
public:
    explicit Script(std::string filePath);
    const std::string &GetFilePath() const;
    void Execute() const;
};
