#include "engine/physics2D/Physics2D.hpp"
#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/util/Setting.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"
#include "engine/core/MainWindow.hpp"

Physics2D::Physics2D() {
    world = new b2World(gravity);
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

    if (circleCollider != nullptr) {
        // 生成椭圆顶点
        int segments = 16;
        std::vector<b2Vec2> vertices(segments);
        float angleIncrement = 2.0f * b2_pi / segments;
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleIncrement;
            b2Vec2 p = {circleCollider->GetRadius() * cosf(angle),
                        circleCollider->GetRadius() * sinf(angle)};
            p = Setting::PHYSICS_SCALE_INV * p;
            vertices[i].Set(p.x, p.y);
        }

        // 分割顶点并创建多个多边形
        for (int i = 0; i < segments; i++) {
            b2Vec2 polygonVertices[3];
            int32 vertexCount = 0;

            // 第一个顶点是多边形的中心
            polygonVertices[vertexCount++] = b2Vec2(0, 0);

            // 复制顶点到多边形
            for (int j = 0; j < 2; ++j) {
                polygonVertices[vertexCount++] = vertices[(i + j) % segments];
            }

            // 创建新的多边形形状
            b2PolygonShape poly;
            poly.Set(polygonVertices, vertexCount);

            // 创建新的fixture
            circleCollider->SetFixture(rawBody->CreateFixture(&poly, rb->GetMass()), i);
        }
    }
    else if (box2DCollider != nullptr) {
        auto *shape_ = new b2PolygonShape();
        glm::vec2 halfSize = Setting::PHYSICS_SCALE_INV * box2DCollider->GetSize() * 0.5f;
        glm::vec2 origin = Setting::PHYSICS_SCALE_INV * cs->transform.position;
        shape_->SetAsBox(halfSize.x, halfSize.y, {origin.x, origin.y},
                         glm::radians(cs->transform.rotation));
        shape = shape_;

        box2DCollider->SetFixture(rawBody->CreateFixture(shape, rb->GetMass()));
    }
}

void Physics2D::Update(float dt) {
    physicsTime += dt;
    if (physicsTime >= physicsTimeStep) {
        world->Step(dt, velocityIterations, positionIterations);
        physicsTime -= physicsTimeStep;
    }
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
