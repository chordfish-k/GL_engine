#pragma once

#include <glm/vec2.hpp>
#include <box2d/b2_shape.h>
#include <box2d/b2_fixture.h>
#include <rttr/registration>
#include "engine/core/ASerializableObj.hpp"

class ColliderShape2D;
class RigidBody2D;

class ACollider : public ASerializableObj{
    RTTR_ENABLE()
protected:
    glm::vec2 offset = {0, 0};
    std::vector<b2Fixture*> fixture;
    ColliderShape2D * colliderShape2D = nullptr;
public:
    virtual ~ACollider();

    virtual void EditorUpdate(float dt);

    glm::vec2 GetOffset();

    std::vector<b2Fixture*> &GetFixture();

    void SetFixture(b2Fixture *fixture, int index = 0);

    virtual bool Imgui() = 0;

    virtual void RefreshShape() = 0;
};
