#pragma once

#include "engine/node/Node.hpp"

class PrefabNode : public Node {
    COMPONENT(PrefabNode)
private:
    std::string prefabFile;
    Node *node = nullptr; // 实际显示的node

public:
    PrefabNode();
    ~PrefabNode() override;
    void SetNode(Node *node);
    Node *GetNode();
    void SetPrefabFile(const std::string &filePath);
    const std::string &GetPrefabFile() const;
    void Start() override;
    void Update(float dt) override;
    void EditorUpdate(float dt) override;
    void Destroy() override;
    json Serialize() override;
    Node *Deserialize(json j) override;
    std::string GetName() override;
    void Imgui() override;
    void SetActive(bool active) override;
    void BindThisToScript(sol::table &table) override;
};
