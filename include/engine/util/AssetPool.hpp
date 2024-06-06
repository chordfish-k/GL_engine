#pragma once

#include "engine/renderer/Spritesheet.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Texture.hpp"
#include <string>
#include <unordered_map>

class AssetPool {
private:
    static std::unordered_map<std::string, Shader *> shaders;
    static std::unordered_map<std::string, Texture *> textures;
    static std::unordered_map<std::string, Spritesheet *> spritesheets;

public:
    static Shader *GetShader(const std::string &resourceName);

    static Texture *GetTexture(const std::string &resourceName);

    static void Clear();
};