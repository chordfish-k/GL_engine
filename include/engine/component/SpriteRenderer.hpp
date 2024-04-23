#pragma once

#include "engine/component/Component.hpp"
#include "engine/util/Print.hpp"

class SpriteRenderer : public Component {
public:
    SpriteRenderer() {};

    void Start() override { util::Println("starting..."); }

    void Update(float dt) {}
};