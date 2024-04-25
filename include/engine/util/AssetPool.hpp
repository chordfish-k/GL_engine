#pragma once

#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include <string>
#include <unordered_map>


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