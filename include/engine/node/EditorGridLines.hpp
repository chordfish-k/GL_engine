#pragma once

#include "engine/node/Node.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/Scene.hpp"
#include "engine/core/Camera.hpp"

COMPONENT(EditorGridLines)
public:
    void Update(float dt) override;
};
