#include "engine/script/LuaNodeBinder.hpp"
#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/physics2D/RigidBody2D.hpp"

void LuaNodeBinder::Bind(sol::state &state) {
    state.new_usertype<Transform>("Transform",
                                  sol::call_constructor,
                                  sol::constructors<Transform(glm::vec2,glm::vec2,float)>(),
                                  "position", &Transform::position,
                                  "rotation", &Transform::rotation,
                                  "scale",    &Transform::scale);

    state.new_usertype<Animation>("Animation",
                                  sol::constructors<Animation>(),
                                  "GetFrame", &Animation::GetFrame,
                                  "SetFrame", &Animation::SetFrame,
                                  "GetHFrames", &Animation::GetHFrames,
                                  "SetHFrames", &Animation::SetHFrames,
                                  "GetVFrames", &Animation::GetVFrames,
                                  "SetVFrames", &Animation::SetVFrames);

    state.new_usertype<Node>("Node",
                             sol::constructors<Node>(),
                             "transform", &Node::transform,
                             "name", &Node::name,
                             "children", &Node::children,
                             "parent", &Node::parent,
                             "Copy", &Node::Copy,
                             "Destroy", &Node::Destroy,
                             "AddChildNode", &Node::AddChildNode,
                             "GetWorldPos", &Node::GetWorldPos,
                             "ToSpriteRenderer", &Node::To<SpriteRenderer>,
                             "ToRigidBody2D", &Node::To<RigidBody2D>
                                 );

    state.new_usertype<SpriteRenderer>("SpriteRenderer",
                                       sol::base_classes, sol::bases<Node>(),
                                       sol::call_constructor, sol::constructors<SpriteRenderer>(),
                                       "GetAnimation", &SpriteRenderer::GetAnimation,
                                       "SetAnimation", &SpriteRenderer::SetAnimation,
                                       "flipX", sol::property(&SpriteRenderer::IsFlipX, &SpriteRenderer::SetFlipX),
                                       "flipY", sol::property(&SpriteRenderer::IsFlipY, &SpriteRenderer::SetFlipY)
                                       );

    state.new_usertype<RigidBody2D>("RigidBody2D",
                                    sol::base_classes, sol::bases<Node>(),
                                    sol::call_constructor, sol::constructors<RigidBody2D>(),
                                    "velocity", sol::property(&RigidBody2D::GetLinearVelocity, &RigidBody2D::SetLinearVelocity),
                                    "angularVelocity", sol::property(&RigidBody2D::GetAngularVelocity, &RigidBody2D::SetAngularVelocity));
}
