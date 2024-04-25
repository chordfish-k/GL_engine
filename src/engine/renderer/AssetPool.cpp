#include "engine/util/AssetPool.hpp"
#include "engine/renderer/Texture.hpp"
#include "engine/util/Print.hpp"

#include <filesystem>

namespace fs = std::filesystem;

std::unordered_map<std::string, Shader *> AssetPool::shaders =
    std::unordered_map<std::string, Shader *>();
std::unordered_map<std::string, Texture *> AssetPool::textures =
    std::unordered_map<std::string, Texture *>();

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
        Texture *texture = new Texture(resourceName);
        textures[path] = texture;
        return texture;
    }
}

std::string AssetPool::GetAbsolutePath(std::string path) {
    fs::path curath = fs::current_path();
    fs::path abPath = curath / path;
    return abPath.string();
}