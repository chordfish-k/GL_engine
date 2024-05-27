#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/renderer/DebugDraw.hpp"

class ColliderShape2D;

class Box2DCollider : public ACollider {
    friend class ColliderShape2D;
private:
    glm::vec2 size = {1, 1};
    ColliderShape2D * colliderShape2D;

public:
    const glm::vec2 &GetSize() const;

    void SetSize(const glm::vec2 &size);

    void EditorUpdate(float dt);

    std::string GetColliderShapeType() override;

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    bool Imgui() override;
};
