#include "engine/physics2D/Physics2D.hpp"
#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/core/MainWindow.hpp"

Physics2D::Physics2D() {
    world = new b2World(gravity);

    debugDraw.SetFlags(b2Draw::e_shapeBit // fixture形状
//                       | b2Draw::e_jointBit // 关节
//                       | b2Draw::e_aabbBit  // 外包围盒
//                       | b2Draw::e_pairBit // 碰撞对连线
                       | b2Draw::e_centerOfMassBit // 质心
                       );
    world->SetDebugDraw(&debugDraw);
}

Physics2D::~Physics2D() {
    delete world;
}

void Physics2D::Add(RigidBody2D *rb) {
    if (rb == nullptr || rb->GetRawBody() != nullptr) return;

    auto pmat = rb->GetModelMatrix();
    Transform temp;
    temp.ApplyDataByLocalMatrix(pmat);
    auto p = temp.position;
    auto ro = glm::radians(temp.rotation);

    b2BodyDef bodyDef;
    bodyDef.angle = ro;
    bodyDef.position = {p.x, p.y};
    bodyDef.position = Setting::PHYSICS_SCALE_INV * bodyDef.position;

    bodyDef.angularDamping = rb->GetAngularDamping();
    bodyDef.linearDamping = rb->GetLinearDamping();
    bodyDef.fixedRotation = rb->IsFixedRotation();
    bodyDef.bullet = rb->IsContinuousCollision();

    auto vel = b2Vec2(rb->GetLinearVelocity().x, rb->GetLinearVelocity().y);
    bodyDef.linearVelocity = Setting::PHYSICS_SCALE_INV * vel;
    bodyDef.angularVelocity = Setting::PHYSICS_SCALE_INV * rb->GetAngularVelocity();

    switch (rb->GetBodyType()) {

    case BodyType::Static:
        bodyDef.type = b2_staticBody;
        break;
    case BodyType::Dynamic:
        bodyDef.type = b2_dynamicBody;
        bodyDef.allowSleep = false;
        break;
    case BodyType::Kinematic:
        bodyDef.type = b2_kinematicBody;
        break;
    }

    b2Body *body = world->CreateBody(&bodyDef);
    body->SetFixedRotation(rb->IsFixedRotation());
    rb->SetRawBody(body);
}

void Physics2D::Add(ColliderShape2D *cs) {
    RigidBody2D *rb = nullptr;
    b2Body *rawBody = nullptr;

    if (!cs->parent) return;
    // 加入到最近rigid父节点的fixtureList中
    Node *p = cs;
    while (p->parent) {
        rb = dynamic_cast<RigidBody2D *>(p->parent);
        if (rb && rb->GetRawBody()) {
            rawBody = rb->GetRawBody();
            cs->SetRigidBody2D(rb);
            break;
        } else {
            p = p->parent;
        }
    }

    if (!rawBody)
        return;

    b2Shape *shape = nullptr;
    auto *colliderShape2D = cs;
    if (colliderShape2D == nullptr)
        return;

    ACollider *collider = colliderShape2D->GetCollider();
    auto *circleCollider = dynamic_cast<CircleCollider*>(collider);
    auto *box2DCollider = dynamic_cast<Box2DCollider*>(collider);

    b2PolygonShape poly;
    poly.SetAsBox(0.001, 0.001);
    // 默认随便给一个fixture
    collider->SetFixture(rawBody->CreateFixture(&poly, rb->GetMass()));
}

void Physics2D::Update(float dt) {
    physicsTime += dt;
    if (physicsTime >= physicsTimeStep) {
        world->Step(dt, velocityIterations, positionIterations);
        physicsTime -= physicsTimeStep;
    }

    if (Setting::PHYSICS_DRAW_DEBUG)
        world->DebugDraw();
}

void Physics2D::DestroyNode(Node *node) {
    if (!node) return;
    // 递归调用，从物理系统删除子节点
    for (auto n : node->children) {
        DestroyNode(n);
    }

    auto rb = dynamic_cast<RigidBody2D*>(node);
    auto cs = dynamic_cast<ColliderShape2D*>(node);

    // 1.如果是RigidBody，直接清除
    if (rb && rb->GetRawBody() && world) {
        rb->SetRawBody(nullptr);
    }
    // 2.如果是ColliderShape，则找到其依附的父节点RigidBody删去ColliderShape的fixture
    else if (cs && cs->GetCollider() && world) {
        auto &fs = cs->GetCollider()->GetFixture();
        if (fs.empty()) return;

        auto *rbb = cs->GetRigidBody2D();
        if (rbb) {
            if (rbb->GetRawBody()) {
                // 循环删除所有fixture
                for (auto &f : fs) {
                    rbb->GetRawBody()->DestroyFixture(f);
                }
            }
        }
    }
}

void Physics2D::ReAdd(RigidBody2D *rb) {
    DestroyNode(rb);
    Add(rb);
}
