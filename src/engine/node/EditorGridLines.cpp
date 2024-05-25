#include "engine/node/EditorGridLines.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/editor/PropertiesWindow.hpp"

void EditorGridLines::EditorUpdate(float dt) {
    Node::Update(dt);

    Node *node = PropertiesWindow::GetActiveNode();
    if (node != nullptr && node->GetNodeType() == "TileMap") {
        ShowTileMapGrids(dynamic_cast<TileMap *>(node));
    }

    auto camera = MainWindow::GetScene()->GetCamera();
    auto cameraPos = camera->position;
    auto cameraZoom = camera->GetZoom();
    auto projSize = camera->GetProjectionSize();

    glm::vec2 center = {0, 0};
    glm::vec2 down = {center.x, (int)(projSize.y * cameraZoom * -.5f + cameraPos.y)};
    glm::vec2 up = {center.x, (int)(projSize.y * cameraZoom * .5f + cameraPos.y)};
    glm::vec2 left = {(int)(projSize.x * cameraZoom * -.5f + cameraPos.x), center.y};
    glm::vec2 right = {(int)(projSize.x * cameraZoom * .5f + cameraPos.x), center.y};

    DebugDraw::AddLine2D(center, up, {0, 0.8, 0, 1});
    DebugDraw::AddLine2D(center, down, {0.3, 0.8, 0.3, 1});
    DebugDraw::AddLine2D(center, left, {0.8, 0, 0, 1});
    DebugDraw::AddLine2D(center, right, {0.8, 0.3, 0.3, 1});


}

void EditorGridLines::ShowTileMapGrids(TileMap *tileMap) {
    int cellW = tileMap->GetCellWidth();
    int cellH = tileMap->GetCellHeight();

    if (cellW == 0 || cellH == 0) return;

    auto camera = MainWindow::GetScene()->GetCamera();
    auto cameraPos = camera->position;
    auto cameraZoom = camera->GetZoom();
    auto projSize = camera->GetProjectionSize();

    int mWH = cellW < cellH ? cellW : cellH;
//    util::Println(cameraZoom / mWH);
    if (cameraZoom / mWH  > 0.12) return;

    int begin = 0;
    int end = (int)(projSize.x * cameraZoom * .5f + cameraPos.x);
    for (int i = begin; i < end; i += cellW) {
        glm::vec2 center = {i, 0};
        glm::vec2 down = {center.x, (int)(projSize.y * cameraZoom * -.5f + cameraPos.y)};
        glm::vec2 up = {center.x, (int)(projSize.y * cameraZoom * .5f + cameraPos.y)};
        DebugDraw::AddLine2D(up, down, {0.45, 0.35, 0.32, 1});
    }

    begin = 0;
    end = (int)(projSize.x * cameraZoom * -.5f + cameraPos.x);
    for (int i = begin; i > end; i -= cellW) {
        glm::vec2 center = {i, 0};
        glm::vec2 down = {center.x, (int)(projSize.y * cameraZoom * -.5f + cameraPos.y)};
        glm::vec2 up = {center.x, (int)(projSize.y * cameraZoom * .5f + cameraPos.y)};
        DebugDraw::AddLine2D(up, down, {0.45, 0.35, 0.32, 1});
    }

    begin = 0;
    end = (int)(projSize.y * cameraZoom * .5f + cameraPos.y);
    for (int i = begin; i < end; i += cellH) {
        glm::vec2 center = {0, i};
        glm::vec2 left = {(int)(projSize.x * cameraZoom * -.5f + cameraPos.x), center.y};
        glm::vec2 right = {(int)(projSize.x * cameraZoom * .5f + cameraPos.x), center.y};
        DebugDraw::AddLine2D(left, right, {0.45, 0.35, 0.32, 1});
    }

    begin = 0;
    end = (int)(projSize.y * cameraZoom * -.5f + cameraPos.y);
    for (int i = begin; i >= end; i -= cellH) {
        glm::vec2 center = {0, i};
        glm::vec2 left = {(int)(projSize.x * cameraZoom * -.5f + cameraPos.x), center.y};
        glm::vec2 right = {(int)(projSize.x * cameraZoom * .5f + cameraPos.x), center.y};
        DebugDraw::AddLine2D(left, right, {0.45, 0.35, 0.32, 1});
    }
}
