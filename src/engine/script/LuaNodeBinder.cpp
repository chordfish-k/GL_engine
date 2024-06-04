#include "engine/script/LuaNodeBinder.hpp"
#include "engine/node/Node.hpp"
#include "engine/node/SpriteRenderer.hpp"

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
                                  "GetHFrame", &Animation::GetHFrames,
                                  "SetHFrame", &Animation::SetHFrames,
                                  "GetVFrame", &Animation::GetVFrames,
                                  "SetVFrame", &Animation::SetVFrames);

    state.new_usertype<Node>("Node",
                             sol::constructors<Node>(),
                             "transform", &Node::transform,
                             "name", &Node::name,
                             "children", &Node::children,
                             "parent", &Node::parent,
                             "GetNode", &Node::GetNode);

    state.new_usertype<SpriteRenderer>("SpriteRenderer",
                                       sol::base_classes, sol::bases<Node>(),
                                       sol::call_constructor, sol::constructors<SpriteRenderer>(),
                                       "GetAnimation", &SpriteRenderer::GetAnimation,
                                       "SetAnimation", &SpriteRenderer::SetAnimation,
                                       "GetNode", &SpriteRenderer::GetNode);
}
