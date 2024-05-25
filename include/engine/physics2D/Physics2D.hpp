#pragma once

#include <box2d/box2d.h>
#include "RigidBody2D.hpp"

class Physics2D {
private:
    b2Vec2 gravity = {0, -98.f};
    b2World world = b2World(gravity);
    float physicsTime = 0.f;
    float physicsTimeStep = 1.f / 60.f;
    int velocityIterations = 8; // 速度求解迭代次数
    int positionIterations = 3; // 位置求解迭代次数

public:
    void Add(RigidBody2D *rb);

    void DestroyNode(Node *node);

    void ReAdd(RigidBody2D *rb);

    void Update(float dt);
};
