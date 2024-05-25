#pragma once

#include "engine/node/Node.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/core/Scene.hpp"
#include "engine/core/Camera.hpp"
#include "TileMap.hpp"

COMPONENT(EditorGridLines)
public:
    void EditorUpdate(float dt) override;

private:
    void ShowTileMapGrids(TileMap *tileMap);
};
