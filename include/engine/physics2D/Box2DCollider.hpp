#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/RigidBody2D.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include <rttr/registration>

class Box2DCollider : public ACollider {
    RTTR_ENABLE(ACollider)
    friend class ColliderShape2D;
private:
    glm::vec2 size = {1, 1};
public:
    Box2DCollider(ColliderShape2D *collider);

    const glm::vec2 &GetSize() const;

    void SetSize(const glm::vec2 &size);

    void EditorUpdate(float dt) override;

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    bool Imgui() override;

    void RefreshShape() override;
};
