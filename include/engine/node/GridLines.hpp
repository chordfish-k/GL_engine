#pragma once

#include "engine/node/Node.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/AScene.hpp"
#include "engine/core/Camera.hpp"

COMPONENT(GridLines)
public:
    void Update(float dt) override;
};
