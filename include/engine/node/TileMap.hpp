#pragma once

#include "Node.hpp"

COMPONENT(TileMap)
protected:
    int cellWidth = 16;
    int cellHeight = 16;

public:

    json Serialize() override {
        json j = Node::Serialize();
        j["data"]["cellSize"] = {cellWidth, cellHeight};
        return j;
    }

    TileMap *Deserialize(json j) override {
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

    void SetCellSize(int cellWidth_, int cellHeight_){
        cellWidth = cellWidth_;
        cellHeight = cellHeight_;
    }

    void SetCellWidth(int cellWidth_) {
        cellWidth = cellWidth_;
    }
    void SetCellHeight(int cellHeight_) {
        cellHeight = cellHeight_;
    }

    int GetCellWidth() const {
        return cellWidth;
    }

    int GetCellHeight() const {
        return cellHeight;
    }

    void Imgui() override {
        Node::Imgui();

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(GetNodeType().c_str())) {
            ShowImgui();
        }
    }
    };
