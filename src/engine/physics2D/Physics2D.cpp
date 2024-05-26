#include "engine/physics2D/Physics2D.hpp"
#include "engine/physics2D/CircleCollider.hpp"
#include "engine/physics2D/Box2DCollider.hpp"

Physics2D::Physics2D() {
    world = new b2World(gravity);
}

Physics2D::~Physics2D() {
    delete world;
}

void Physics2D::Add(RigidBody2D *rb) {
    if (rb == nullptr || rb->GetRawBody() != nullptr) return;

    Transform &transform = rb->transform;

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

    b2PolygonShape shape;

    ACollider *collider = rb->GetCollider();
    auto *circleCollider = dynamic_cast<CircleCollider*>(collider);
    auto *box2DCollider = dynamic_cast<Box2DCollider*>(collider);
    if (circleCollider != nullptr) {

    }
    else if (box2DCollider != nullptr) {
        glm::vec2 halfSize = box2DCollider->GetHalfSize();
        glm::vec2 offset = box2DCollider->GetOffset();
        glm::vec2 origin = box2DCollider->GetOrigin();
        shape.SetAsBox(halfSize.x, halfSize.y, {origin.x, origin.y}, 0);

        b2Vec2 pos = bodyDef.position;
        float  xPos = pos.x + offset.x;
        float  yPos = pos.y + offset.y;
        bodyDef.position = {xPos, yPos};
    }

    b2Body *body = world->CreateBody(&bodyDef);
    rb->SetRawBody(body);
    body->CreateFixture(&shape, rb->GetMass());
    body->SetFixedRotation(rb->IsFixedRotation());
}

void Physics2D::Update(float dt) {
    physicsTime += dt;
    if (physicsTime >= physicsTimeStep) {
        world->Step(physicsTime, velocityIterations, positionIterations);
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
}
void Physics2D::ReAdd(RigidBody2D *rb) {
    DestroyNode(rb);
    Add(rb);
}
