#include "engine/util/AssetPool.hpp"
#include "engine/util/Print.hpp"
#include "engine/util/Common.hpp"

#include <cassert>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::unordered_map<std::string, Shader *> AssetPool::shaders =
    std::unordered_map<std::string, Shader *>();
std::unordered_map<std::string, Texture *> AssetPool::textures =
    std::unordered_map<std::string, Texture *>();

Shader *AssetPool::GetShader(const std::string &resourceName) {
    // 如果一句载入过，则返回，否则载入再返回
    std::string path = util::GetAbsolutePath(resourceName);
    if (shaders.count(path)) {
        return shaders[path];
    } else {
        Shader *shader = new Shader(path);
        shader->Compile();
        shaders[path] = shader;
        return shader;
    }
}

Texture *AssetPool::GetTexture(const std::string &resourceName) {
    // 如果一句载入过，则返回，否则 载入再返回
    std::string path = util::GetAbsolutePath(resourceName);
    if (textures.count(path)) {
        return textures[path];
    } else {
        auto *texture = new Texture();
        bool succeed = texture->Init(path);
        if (!succeed) return nullptr;
        textures[path] = texture;
        return texture;
    }
}

void AssetPool::Clear() {
    for (auto item : shaders) {
        if (item.second != nullptr) {
            delete item.second;
            item.second = nullptr;
        }
    }
    shaders.clear();


    for (auto item : textures) {
        if (item.second != nullptr) {
            delete item.second;
            item.second = nullptr;
        }
    }
    textures.clear();
}