#pragma once

#include <rttr/type.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <rttr/registration>
#include "engine/core/ASerializableObj.hpp"
#include "engine/util/Print.hpp"
#include "engine/core/Transform.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/editor/MyImGui.hpp"
#include "engine/renderer/Color.hpp"
#include "engine/core/ZIndex.hpp"
#include "engine/util/AssetPool.hpp"
#include "engine/core/AGuiObj.hpp"

class Node : public ASerializableObj{
    RTTR_ENABLE()
private:
    static int ID_COUNTER;
    int uid = -1;

    bool doSerialization = true;
    bool isPickable = true;
    bool shouldDestroy = false;

protected:
    ZIndex zIndex = 0;
    bool active = true;

public:
    Node *parent = nullptr;
    std::list<Node *> children;
    Transform transform;
    const std::string nodeType = "Node";
    std::string name = "";

public:
    Node() : Node(glm::vec2(), glm::vec2(1,1)){};

    Node(glm::vec2 position, glm::vec2 scale = glm::vec2(1,1));

    virtual ~Node();

    void Delete();

    virtual std::string GetNodeType() {return nodeType;}

    static void Init(int maxId) {ID_COUNTER = maxId; }

    virtual void Start();

    virtual void Update(float dt);

    virtual void EditorUpdate(float dt);

    void CheckDelete();

    virtual void Destroy() {
        shouldDestroy = true;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of<Node, T>::value, T *>
    AddNode(const std::string &name_ = "") {
        T *comp = new T();
        comp->name = name_;
        AddNode(comp);
        return comp;
    }

    Node *AddNode(Node *comp);

    void RemoveAllNodes();

    json Serialize() override;

    Node *Deserialize(json j) override;

    void TravelOnSubTree(const std::function<void(Node*)>&);

    bool IsChildOf(Node *p);

    void GeneratedId(bool force=false);

    int GetUid() const { return uid; }

    int GetZIndex() const { return zIndex.GetZIndex(); }

    glm::mat4 GetModelMatrix() const;

    glm::mat4 GetModelMatrixRelativeTo(Node *by) const;

    bool ShouldDestroy() const;

    bool IsPickable() const;

    void SetIsPickable(bool isPickable);

    virtual bool IsActive() const;

    virtual void SetActive(bool active);

    bool IsDoSerialization() const { return doSerialization; }

    Node *SetDoSerialization(bool enable) { doSerialization = enable; return this; }

    Transform GetTransform() const { return parent ? parent->GetTransform() + transform : transform;}

    virtual std::string GetName() { return name == "" ? GetNodeType() : name; }

    Node *SetName(const std::string &name_) {
        this->name = name_;
        return this;
    }

    Node *SetZIndex(int zIndex_) {
        zIndex_ = zIndex_ >= 0 ? zIndex_ : 0;
        this->zIndex = zIndex_ <= 100 ? zIndex_ : 100;
        return this;
    }

    Node *SetPosition(glm::vec2 position) {
        transform.position = position;
        return this;
    }

    Node *SetScale(glm::vec2 scale) {
        transform.scale = scale;
        return this;
    }

    Node *SetRotation(float rotation) {
        transform.rotation = rotation;
        return this;
    }

    Node *SetTransform(Transform t) {
        transform = t;
        return this;
    }
public:
    virtual void Imgui();

protected:
    void ShowImgui(std::vector<std::string> notShowFields = {});
};

#define COMPONENT(name_)                                        \
    public:                                                     \
        const std::string nodeType = #name_;                    \
        std::string GetNodeType() override {return nodeType;}   \
    private:
