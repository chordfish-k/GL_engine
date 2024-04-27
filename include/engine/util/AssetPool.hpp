#pragma once

#include "engine/component/Spritesheet.hpp"
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
    static Shader *GetShader(std::string resourceName);

    static Texture *GetTexture(std::string resourceName);

    static void AddSpritesheet(std::string resourceName,
                               Spritesheet *spritesheet);

    static Spritesheet *GetSpritesheet(std::string resourceName);

    static void Clear();

private:
    static std::string GetAbsolutePath(std::string path);
};