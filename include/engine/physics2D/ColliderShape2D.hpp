#pragma once

#include "engine/physics2D/ACollider.hpp"
#include "engine/physics2D/ShapeType.hpp"
#include "engine/node/Node.hpp"

class ColliderShape2D : public Node{
    COMPONENT(ColliderShape2D)
    RTTR_ENABLE(Node)
private:
    ACollider *collider = nullptr;
    RigidBody2D *rigidBody2D = nullptr;
    Transform lastTransform;
    glm::vec2 lastPos;
    ShapeType shapeType = ShapeType::Box2DCollider;
    bool sensor = false;
public:
    ColliderShape2D();

    ~ColliderShape2D() override;

    ACollider *GetCollider() const;

    void SetCollider(ACollider *shape);

    RigidBody2D *GetRigidBody2D() const;

    void SetRigidBody2D(RigidBody2D *rigidBody2D);

    bool IsSensor() const;

    void SetSensor(bool sensor);

    void Start() override;

    json Serialize() override;

    Node *Deserialize(json j) override;

    void Imgui() override;

    void Update(float dt) override;

    void EditorUpdate(float dt) override;

    ShapeType GetShapeType() const;

    void SetShapeType(ShapeType shapeType);

    void BindThisToScript(sol::table &table) override;

private:
    void TryReshape();
};
