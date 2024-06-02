#pragma once

#include "engine/node/Node.hpp"

struct TileCell;
class TileMap;

struct TileSet {
    Texture *texture;
    int rows;
    int columns;
    TileSet(Texture *pTexture, int rows, int columns);
private:
    int cellWidth;
    int cellHeight;
    friend struct TileCell;
    friend class TileMap;
};

struct TileCell {
    friend class TileMap;
private:
    int tileX = 0;
    int tileY = 0;
    int index = 0;
    int tileSetIndex = 0;
    TileMap *tileMapPtr = nullptr;
    TileSet *tileSetPtr = nullptr;
    SpriteRenderer *spriteRenderer = nullptr;
public:
    void SetIndex(int index_);
    void SetPosition(int tileX_, int tileY_);
};

class TileMap : public Node {
    COMPONENT(TileMap)
    RTTR_ENABLE(Node)
protected:
    int cellWidth = 16;
    int cellHeight = 16;
    static std::vector<TileSet> tileSetList;
    std::vector<TileCell> tileList;
    TileCell cursorTile;
    bool hasChosenTile = false;
    bool firstSetCursorTile = true;

public:
    TileMap();

    virtual ~TileMap();

    json Serialize() override;

    TileMap *Deserialize(json j) override;

    void SetCellSize(int cellWidth_, int cellHeight_);

    void SetCellWidth(int cellWidth_);

    void SetCellHeight(int cellHeight_);

    int GetCellWidth() const;

    int GetCellHeight() const;

    void Imgui() override;

    const std::vector<TileSet> &GetTileSetList() const;

    void AddTileSet(TileSet tileSet);

    void AddTileCell(int x, int y, int tileSetIndex=0, int tileIndex=0);

    void Update(float dt) override;

    void EditorUpdate(float dt) override;

private:
    void SetCursorTile(int tileSetIndex, int tileIndex);

    void ShowTileMapGrids();
};
