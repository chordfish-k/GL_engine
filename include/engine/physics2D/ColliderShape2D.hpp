#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/ShapeType.hpp"
#include "engine/node/Node.hpp"

class ColliderShape2D : public Node{
    COMPONENT(ColliderShape2D)
    RTTR_ENABLE(Node)
private:
    ACollider *collider = nullptr;

    Transform lastTransform;
    ShapeType shapeType = ShapeType::Box2DCollider;
public:
    ColliderShape2D();

    ~ColliderShape2D() override;

    ACollider *GetCollider() const;

    void SetCollider(ACollider *shape);

    void Start() override;

    json Serialize() override;

    Node *Deserialize(json j) override;

    void Imgui() override;

    void Update(float dt) override;

    void EditorUpdate(float dt) override;

    ShapeType GetShapeType() const;

    void SetShapeType(ShapeType shapeType);

private:
    void RefreshShapeByTransform();
};
