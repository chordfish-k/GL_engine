#include "engine/node/TileMap.hpp"

json TileMap::Serialize(){
    json j = Node::Serialize();
    j["data"]["cellSize"] = {cellWidth, cellHeight};
    return j;
}

TileMap *TileMap::Deserialize(json j){
    Node::Deserialize(j);
    auto &data = j["data"];
    if (data.empty()) return this;

    auto &cs = data["cellSize"];
    if (!cs.empty() && cs.size() == 2) {
        cellWidth = cs[0];
        cellHeight = cs[1];
    }

    return this;
}

void TileMap::SetCellSize(int cellWidth_, int cellHeight_){
    cellWidth = cellWidth_;
    cellHeight = cellHeight_;
}

void TileMap::SetCellWidth(int cellWidth_) {
    cellWidth = cellWidth_;
}
void TileMap::SetCellHeight(int cellHeight_) {
    cellHeight = cellHeight_;
}

int TileMap::GetCellWidth() const {
    return cellWidth;
}

int TileMap::GetCellHeight() const {
    return cellHeight;
}

void TileMap::Imgui(){
    Node::Imgui();
    ImGui::Dummy({1,2});
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(GetNodeType().c_str())) {
        ShowImgui();
    }
}