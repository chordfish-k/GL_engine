#include "engine/physics2D/Physics2D.hpp"
#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/Box2DCollider.hpp"
#include "engine/util/Setting.hpp"
#include "engine/physics2D/ColliderShape2D.hpp"

Physics2D::Physics2D() {
    world = new b2World(gravity);
}

Physics2D::~Physics2D() {
    delete world;
}

void Physics2D::Add(RigidBody2D *rb) {
    if (rb == nullptr || rb->GetRawBody() != nullptr) return;

    Transform transform = rb->GetTransform();

    b2BodyDef bodyDef;
    bodyDef.angle = (float) glm::radians(transform.rotation);
    bodyDef.position = {transform.position.x, transform.position.y};
    bodyDef.angularDamping = rb->GetAngularDamping();
    bodyDef.linearDamping = rb->GetLinearDamping();
    bodyDef.fixedRotation = rb->IsFixedRotation();
    bodyDef.bullet = rb->IsContinuousCollision();

    switch (rb->GetBodyType()) {

    case BodyType::Static:
        bodyDef.type = b2_staticBody;
        break;
    case BodyType::Dynamic:
        bodyDef.type = b2_dynamicBody;
        break;
    case BodyType::Kinematic:
        bodyDef.type = b2_kinematicBody;
        break;
    }

    b2Vec2 pos = bodyDef.position;
    float  xPos = pos.x;
    float  yPos = pos.y;
    bodyDef.position = Setting::PHYSICS_SCALE_INV * b2Vec2(xPos, yPos);

    b2Body *body = world->CreateBody(&bodyDef);

    for (auto *ch : rb->children) {
        b2Shape *shape = nullptr;
        auto *colliderShape2D = dynamic_cast<ColliderShape2D*>(ch);
        if (colliderShape2D == nullptr)
            continue;

        ACollider *collider = colliderShape2D->GetCollider();
        auto *circleCollider = dynamic_cast<CircleCollider*>(collider);
        auto *box2DCollider = dynamic_cast<Box2DCollider*>(collider);
        if (circleCollider != nullptr) {

        }
        else if (box2DCollider != nullptr) {
            auto *shape_ = new b2PolygonShape();
            glm::vec2 halfSize =
                box2DCollider->GetSize() * Setting::PHYSICS_SCALE_INV;
//            glm::vec2 offset = box2DCollider->GetOffset();
            glm::vec2 origin = Setting::PHYSICS_SCALE_INV * box2DCollider->GetOrigin();
            shape_->SetAsBox(halfSize.x, halfSize.y, {origin.x, origin.y}, 0);
            shape = shape_;
        }
        auto fixture = body->CreateFixture(shape, rb->GetMass());
        collider->SetFixture(fixture);
    }
    body->SetFixedRotation(rb->IsFixedRotation());
    rb->SetRawBody(body);
}

void Physics2D::Add(ColliderShape2D *cs) {
    b2Shape *shape = nullptr;
    auto *colliderShape2D = cs;
    if (colliderShape2D == nullptr)
        return;

    ACollider *collider = colliderShape2D->GetCollider();
    auto *circleCollider = dynamic_cast<CircleCollider*>(collider);
    auto *box2DCollider = dynamic_cast<Box2DCollider*>(collider);
    if (circleCollider != nullptr) {

    }
    else if (box2DCollider != nullptr) {
        auto *shape_ = new b2PolygonShape();
        glm::vec2 halfSize =
            box2DCollider->GetSize() * Setting::PHYSICS_SCALE_INV;
        glm::vec2 origin = Setting::PHYSICS_SCALE_INV * box2DCollider->GetOrigin();
        shape_->SetAsBox(halfSize.x, halfSize.y, {origin.x, origin.y}, 0);
        shape = shape_;
    }

    if (!cs->parent) return;
    auto rb = dynamic_cast<RigidBody2D*>(cs->parent);
    if (rb->GetRawBody()) {
        auto fixture = rb->GetRawBody()->CreateFixture(shape, rb->GetMass());
        collider->SetFixture(fixture);
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
    for (auto n : node->children) {
        DestroyNode(n);
    }

    auto rb = dynamic_cast<RigidBody2D*>(node);
    if (rb && rb->GetRawBody() && world) {
        world->DestroyBody(rb->GetRawBody());
        rb->SetRawBody(nullptr);
    }

    auto cs = dynamic_cast<ColliderShape2D*>(node);
    if (cs && cs->GetCollider() && cs->GetCollider()->GetFixture() && world) {
        auto parent = cs->parent;
        auto rbb = dynamic_cast<RigidBody2D*>(parent);
        if (rbb && rbb->GetRawBody()) {
            rbb->GetRawBody()->DestroyFixture(cs->GetCollider()->GetFixture());
        }
    }
}

void Physics2D::ReAdd(RigidBody2D *rb) {
    DestroyNode(rb);
    Add(rb);
}
