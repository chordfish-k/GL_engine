#pragma once

#include "engine/node/Node.hpp"

class TileMap : public Node {
    COMPONENT(TileMap)
protected:
    int cellWidth = 16;
    int cellHeight = 16;

public:

    json Serialize() override;

    TileMap *Deserialize(json j) override;

    void SetCellSize(int cellWidth_, int cellHeight_);

    void SetCellWidth(int cellWidth_);

    void SetCellHeight(int cellHeight_);

    int GetCellWidth() const;

    int GetCellHeight() const;

    void Imgui() override;
};
