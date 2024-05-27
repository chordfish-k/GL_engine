#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/renderer/DebugDraw.hpp"

class ColliderShape2D;

class Box2DCollider : public ACollider {
    friend class ColliderShape2D;
private:
    glm::vec2 size = {1, 1};
    glm::vec2 origin = {0, 0};
    ColliderShape2D * colliderShape2D;

public:
    const glm::vec2 &GetSize() const;

    void SetSize(const glm::vec2 &size);

    const glm::vec2 &GetOrigin() const;

    void SetOrigin(const glm::vec2 &origin);

    void EditorUpdate(float dt);

    std::string GetColliderShapeType() override;

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    bool Imgui() override;
};
