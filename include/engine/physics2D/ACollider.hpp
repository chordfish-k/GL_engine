#pragma once

#include <glm/vec2.hpp>
#include <box2d/b2_shape.h>
#include <box2d/b2_fixture.h>
#include "engine/core/ASerializableObj.hpp"

class RigidBody2D;

class ACollider : public ASerializableObj{
protected:
    glm::vec2 offset = {0, 0};
    b2Fixture *fixture = nullptr;
public:
    virtual ~ACollider();

    virtual void EditorUpdate(float dt);

    virtual std::string GetColliderShapeType() = 0;

    glm::vec2 GetOffset();

    b2Fixture *GetFixture() const;

    void SetFixture(b2Fixture *fixture);

    virtual bool Imgui() = 0;
};
