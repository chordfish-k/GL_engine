﻿#include "engine/util/AssetPool.hpp"
#include "engine/component/Spritesheet.hpp"
#include "engine/renderer/Texture.hpp"

#include <cassert>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::unordered_map<std::string, Shader *> AssetPool::shaders =
    std::unordered_map<std::string, Shader *>();
std::unordered_map<std::string, Texture *> AssetPool::textures =
    std::unordered_map<std::string, Texture *>();
std::unordered_map<std::string, Spritesheet *> AssetPool::spritesheets =
    std::unordered_map<std::string, Spritesheet *>();

Shader *AssetPool::GetShader(std::string resourceName) {
    // 如果一句载入过，则返回，否则载入再返回
    std::string path = GetAbsolutePath(resourceName);
    if (shaders.count(path)) {
        return shaders[path];
    } else {
        Shader *shader = new Shader(resourceName);
        shader->Compile();
        shaders[path] = shader;
        return shader;
    }
}

Texture *AssetPool::GetTexture(std::string resourceName) {
    // 如果一句载入过，则返回，否则 载入再返回
    std::string path = GetAbsolutePath(resourceName);
    if (textures.count(path)) {
        return textures[path];
    } else {
        Texture *texture = new Texture();
        texture->Init(resourceName);
        textures[path] = texture;
        return texture;
    }
}

void AssetPool::AddSpritesheet(std::string resourceName,
                               Spritesheet *spritesheet) {
    std::string path = GetAbsolutePath(resourceName);
    if (!spritesheets.count(path)) {
        spritesheets[path] = spritesheet;
    }
}

Spritesheet *AssetPool::GetSpritesheet(std::string resourceName) {
    // 如果一句载入过，则返回，否则 载入再返回
    std::string path = GetAbsolutePath(resourceName);
    if (!spritesheets.count(path)) {
        std::string msg = "ERROR: Tried to access spritesheet '" +
                          resourceName +
                          "' and it has not been added to asset pool.";
        assert(false && msg.c_str());
    }
    return spritesheets[path];
}

std::string AssetPool::GetAbsolutePath(std::string path) {
    fs::path curath = fs::current_path();
    fs::path abPath = curath / path;
    return abPath.string();
}

void AssetPool::Clear() {
    for (auto item : shaders) {
        if (item.second != nullptr) {
            delete item.second;
            item.second = nullptr;
        }
    }
    shaders.clear();

    for (auto item : spritesheets) {
        if (item.second != nullptr) {
            delete item.second;
            item.second = nullptr;
        }
    }
    spritesheets.clear();

    for (auto item : textures) {
        if (item.second != nullptr) {
            delete item.second;
            item.second = nullptr;
        }
    }
    textures.clear();
}