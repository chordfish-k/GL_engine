#pragma once

#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include "engine/util/Print.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

class AssetPool {
private:
    static std::unordered_map<std::string, Shader *> shaders;
    static std::unordered_map<std::string, Texture *> textures;

public:
    static Shader *GetShader(std::string resourceName);

    static Texture *GetTexture(std::string resourceName);

private:
    static std::string GetAbsolutePath(std::string path);
};