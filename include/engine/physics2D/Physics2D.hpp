#pragma once

#include <box2d/box2d.h>
#include "RigidBody2D.hpp"
#include "ColliderShape2D.hpp"
#include "engine/renderer/Box2dDebugDraw.hpp"

class Physics2D {
private:
    b2Vec2 gravity = {0, -9.8f};
    b2World *world = nullptr;
    float physicsTime = 0.f;
    float physicsTimeStep = 1.f / 60.f;
    int velocityIterations = 8; // 速度求解迭代次数
    int positionIterations = 3; // 位置求解迭代次数
    Box2DDebugDraw debugDraw;

public:
    Physics2D();

    ~Physics2D();

    void Add(RigidBody2D *rb);

    void Add(ColliderShape2D *cs);

    void DestroyNode(Node *node);

    void ReAdd(RigidBody2D *rb);

    void Update(float dt);
    void Init();
};
