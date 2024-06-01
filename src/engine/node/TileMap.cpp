#include "engine/node/TileMap.hpp"
#include "engine/node/SpriteRenderer.hpp"

void TileCell::SetIndex(int index_) {
    index = index_;
    if (spriteRenderer != nullptr) {
        spriteRenderer->GetAnimation()->SetFrame(index);
    }
}

void TileCell::SetPosition(int tileX_, int tileY_) {
    tileX = tileX_;
    tileY = tileY_;
    if (spriteRenderer != nullptr) {
        spriteRenderer->transform.position = {
            (tileX + 0.5) * tileMapPtr->GetCellWidth(),
            (tileY + 0.5) * tileMapPtr->GetCellHeight()
        };
    }
}

TileSet::TileSet(Texture *pTexture, int rows, int columns)
    : texture(pTexture), rows(rows), columns(columns){}

TileMap::TileMap() {
    // Test Only
//    AddTileSet({
//        AssetPool::GetTexture("assets/image/spritesheets/decorationsAndBlocks.png"),
//        12, 7
//    });
//    AddTileCell(0, 0, 0, 0);
//    AddTileCell(1, 0, 0, 1);
//    AddTileCell(0, 1, 0, 2);
//    AddTileCell(-1, 0, 0, 3);
//    AddTileCell(0, -1, 0, 4);
}

TileMap::~TileMap() {
    // 删除所有new创建的SpriteRenderer
    for (auto &t : tileList) {
        delete t.spriteRenderer; // 自动从渲染器删除spriteRenderer
        t.spriteRenderer = nullptr;
    }
}

json TileMap::Serialize(){
    json j = Node::Serialize();
    j["data"]["cellSize"] = {cellWidth, cellHeight};

    int index = 0;
    for (auto & ts : tileSetList) {
        j["data"]["tileSets"][index] = {ts.texture->GetFilePath(), ts.rows, ts.columns};
        ++index;
    }

    index = 0;
    for (auto & t : tileList) {
        j["data"]["tiles"][index] = {t.tileX, t.tileY, t.tileSetIndex, t.index};
        ++index;
    }

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

    // 载入tileSet
    auto &tss = data["tileSets"];
    if (!tss.empty()) {
        for (auto &ts : tss) {
            if (ts.size() != 3)
                continue;
            AddTileSet({AssetPool::GetTexture(ts[0]),
                        ts[1], ts[2]});
        }
    }

    // 载入tileCell
    auto &tcs = data["tiles"];
    if (!tcs.empty()) {
        for (auto &tc : tcs) {
            if (tc.size() != 4)
                continue;
            AddTileCell(tc[0], tc[1], tc[2], tc[3]);
        }
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

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        if (ImGui::TreeNodeEx("TileSets",
                              ImGuiTreeNodeFlags_Selected |
                                  ImGuiTreeNodeFlags_DefaultOpen)) {

            static std::string path;
            if (MyImGui::DrawResourceDragDropBoxWithBtn("add from",path, "Add")) {
                AddTileSet({AssetPool::GetTexture(path), 1, 1});
            }

            ImGui::Indent(3);
            int index = 0;
            static int selectTsIndex = 0;
            for (auto &ts : tileSetList) {
                std::string label = "TileSet " + std::to_string(index);

                bool open = ImGui::TreeNodeEx(label.c_str());
                if (open) {
                    std::filesystem::path p = ts.texture->GetFilePath();
                    auto text = p.stem().string();
                    if (MyImGui::DrawButton("texture", text)) {
                        FileSystemWindow::localPath = p.parent_path();
                    }
                    MyImGui::DrawIntSpinner("columns", ts.columns, 1);
                    MyImGui::DrawIntSpinner("rows", ts.rows, 1);

                    std::string treeLabel = "tiles##" + std::to_string(index);
                    if (ImGui::TreeNodeEx(treeLabel.c_str())){

                        int spNum = ts.rows * ts.columns;
                        float width = ImGui::GetContentRegionAvail().x -
                                      ImGui::GetStyle().ScrollbarSize;
                        ImVec2 tsSize = {width,
                                         width / ts.texture->GetWidth() *
                                         ts.texture->GetHeight()};
                        float btnWidth = fmin(32, tsSize.x / ts.columns);
                        ImVec2 btnSize = {btnWidth, btnWidth /
                                                    ts.cellWidth *
                                                    ts.cellHeight};
                        ImVec2 windowPos = ImGui::GetWindowPos();
                        ImVec2 windowSize = ImGui::GetWindowSize();
                        float windowWidthX2 = windowPos.x + windowSize.x;
                        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;

                        if (ImGui::BeginChild("tiles-child")) {

                            float borderSize = 2;
                            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                                                {borderSize, borderSize});
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                                                {2, 2});
                            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,
                                                borderSize);
                            // 显示图片按钮
                            static int btnChoose = -1;
                            for (int i = 0; i < spNum; ++i) {
                                int currentX = (i % ts.columns) * ts.cellWidth;
                                int currentY = (ts.rows - (i / ts.columns)-1) * ts.cellHeight;
                                // 将坐标缩到[0,1]区间，作为texCoords
                                float topY = (currentY + ts.cellHeight) /
                                             (float)ts.texture->GetHeight();
                                float rightX = (currentX + ts.cellWidth) /
                                               (float)ts.texture->GetWidth();
                                float leftX =
                                    currentX / (float)ts.texture->GetWidth();
                                float bottomY =
                                    currentY / (float)ts.texture->GetHeight();

                                std::vector<glm::vec2> texCoords = {
                                    glm::vec2(rightX, topY),
                                    glm::vec2(rightX, bottomY),
                                    glm::vec2(leftX, bottomY),
                                    glm::vec2(leftX, topY),
                                };

                                ImVec2 uv0 = {leftX, topY};
                                ImVec2 uv1 = {rightX, bottomY};

                                bool selected = btnChoose == i && selectTsIndex == index;
                                if (selected)
                                    ImGui::PushStyleColor(ImGuiCol_Border, {0.95, 0.9, 0.15, 1});
                                ImGui::PushID(util::Concat("tsbtnid.", index, ".", i).c_str());
                                if (ImGui::ImageButton(
                                    reinterpret_cast<ImTextureID>(
                                        ts.texture->GetId()),
                                    btnSize, uv0, uv1)) {
                                    selectTsIndex = index;
                                    btnChoose = i;
                                }
                                ImGui::PopID();
                                if (selected)
                                    ImGui::PopStyleColor();

                                // 判断是否要换行
                                ImVec2 lastButtonPos = ImGui::GetItemRectMax();
                                float nextButtonX2 = lastButtonPos.x + itemSpacing.x + btnWidth*2;
                                if (i + 1 < spNum && nextButtonX2 < windowWidthX2) {
                                    ImGui::SameLine();
                                }
                            }
                            ImGui::PopStyleVar(3);
                            ImGui::EndChild();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.2, 0.2, 1});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.6, 0.1, 0.15, 1});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.7, 0.2, 0.2, 1});
                    if (ImGui::Button("Delete", {ImGui::GetContentRegionAvail().x,
                                                 ImGui::GetTextLineHeight()+ImGui::GetStyle().FramePadding.y*2})) {
                        // TODO 删除tileSet

                    }
                    ImGui::PopStyleColor(3);
                    ImGui::TreePop();
                }
                ++index;
            }
            ImGui::Indent();
            ImGui::TreePop();
        }
        ImGui::PopStyleColor();
    }
}

const std::vector<TileSet> &TileMap::GetTileSetList() const {
    return tileSetList;
}

void TileMap::AddTileSet(TileSet tileSet) {
    if (tileSet.texture) {
        tileSet.cellWidth = (int)tileSet.texture->GetWidth() / tileSet.columns;
        tileSet.cellHeight = (int)tileSet.texture->GetHeight() / tileSet.rows;
        tileSetList.push_back(tileSet);
    }
}

void TileMap::AddTileCell(int x, int y, int tileSetIndex, int tileIndex) {
    if (tileSetIndex >= tileSetList.size()) return;

    TileCell cell;
    cell.tileMapPtr = this;

    TileSet &tileSet = tileSetList[tileSetIndex];
    cell.tileSetPtr = &tileSet;

    cell.spriteRenderer = new SpriteRenderer();
    auto spr = cell.spriteRenderer;
    spr->SetTexture(tileSet.texture);

    auto ani = spr->GetAnimation();
    ani->SetFrame(tileIndex);
    ani->SetVFrames(tileSet.rows);
    ani->SetHFrames(tileSet.columns);

    auto &tr = spr->transform;

    // 设置position
    tr.position = {
        (x + 0.5) * GetCellWidth() * GetTransform().scale.x,
        (y + 0.5) * GetCellHeight() * GetTransform().scale.y
    };

    // 设置scale，使其与cellSize吻合
    tr.scale = {
        tileSet.cellWidth / (tileSet.texture->GetWidth() / tileSet.columns),
        tileSet.cellHeight / (tileSet.texture->GetHeight() / tileSet.rows)
    };

    spr->Start(); // 将spriteRenderer加入渲染器
    spr->parent = this; // 设置父节点为自身，让GetTransform和GetMatrix等正常工作

    // tileCell加入数组
    tileList.push_back(cell);
}

void TileMap::Update(float dt) {

    for (auto &t : tileList) {
        // 统一设置z-index
        if (zIndex.GetZIndex() != t.spriteRenderer->GetZIndex()) {
            t.spriteRenderer->SetZIndex(zIndex.GetZIndex());
        }
        t.spriteRenderer->Update(dt);
    }
    Node::Update(dt);
}

void TileMap::EditorUpdate(float dt) {
    for (auto &t : tileList) {
        // 统一设置z-index
        if (zIndex.GetZIndex() != t.spriteRenderer->GetZIndex()) {
            t.spriteRenderer->SetZIndex(zIndex.GetZIndex());
        }
        t.spriteRenderer->EditorUpdate(dt);
    }
    Node::EditorUpdate(dt);
}


