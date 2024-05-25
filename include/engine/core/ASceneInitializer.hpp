#pragma once

class Scene;

class ASceneInitializer {
protected:
    std::string filePath = "";

public:
    ASceneInitializer(const std::string &filePath = ""):filePath(filePath){};
    virtual ~ASceneInitializer() = default;
    virtual void Init(Scene *scene) = 0;
    virtual void LoadResources(Scene *scene) = 0;
    virtual void Imgui() = 0;
    std::string GetFilePath() {return filePath;}
    void SetFilePath(const std::string &filePath) {this->filePath = filePath;}
};
