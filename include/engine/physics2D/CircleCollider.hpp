#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "ColliderShape2D.hpp"
#include <rttr/registration>

#define CIRCLE_SEGMENTS 16
#define CIRCLE_SEGMENTS_MAX 32

class CircleCollider : public ACollider{
    RTTR_ENABLE(ACollider)
private:
    float radius = 1.f;
    b2Vec2 vs[CIRCLE_SEGMENTS_MAX+1];
    int segments = CIRCLE_SEGMENTS;
public:
    CircleCollider(ColliderShape2D *collider);

    float GetRadius() const;

    void SetRadius(float radius);

    void EditorUpdate(float dt) override;

    json Serialize() override;

    ASerializableObj *Deserialize(json j) override;

    bool Imgui() override;

    void RefreshShape() override;
};
