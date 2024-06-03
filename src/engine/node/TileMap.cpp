#include "engine/node/TileMap.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/GameViewWindow.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/core/MainWindow.hpp"
#include "engine/core/Camera.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/util/Mat4Utils.hpp"
#include "engine/core/KeyListener.hpp"
#include "engine/physics2D/Box2DCollider.hpp"

// std::vector<TileSet> TileMap::tileSetList;

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
    rigidBody2D = new RigidBody2D();
    rigidBody2D->parent = this;
    rigidBody2D->transform.position = {0, 0};
    rigidBody2D->SetBodyType(BodyType::Static);
}

TileMap::~TileMap() {
    MainWindow::GetScene()->GetPhysics2D()->DestroyNode(rigidBody2D);
    delete rigidBody2D;

    // 删除所有new创建的SpriteRenderer
//    for (auto &t : tileList) {
//        MainWindow::GetScene()->GetRenderer()->Remove(t.spriteRenderer);
//        delete t.spriteRenderer; // 自动从渲染器删除spriteRenderer
//    }
    if (hasChosenTile) {
        MainWindow::GetScene()->GetRenderer()->Remove(cursorTile.spriteRenderer);
        delete cursorTile.spriteRenderer;
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

    j["data"]["rigidBody"]["bodyType"] = GetNameByBodyType(rigidBody2D->GetBodyType());
    j["data"]["rigidBody"]["fixedRotation"] = rigidBody2D->IsFixedRotation();
    j["data"]["rigidBody"]["mass"] = rigidBody2D->GetMass();

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
        tileSetList.clear();
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
        tileList.clear();
        for (auto &tc : tcs) {
            if (tc.size() != 4)
                continue;
            AddTileCell(tc[0], tc[1], tc[2], tc[3]);
        }
    }

    // 设置内置的rigidBody
    auto &rb = data["rigidBody"];
    if (!rb.empty()) {
        auto &bt = rb["bodyType"];
        if (!bt.empty()) {
            rigidBody2D->SetBodyType(GetBodyTypeByName(bt));
        }
        auto &fr = rb["fixedRotation"];
        if (!fr.empty()) {
            rigidBody2D->SetFixedRotation(fr);
        }
        auto &ma = rb["mass"];
        if (!ma.empty()) {
            rigidBody2D->SetMass(ma);
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
        int lastCellW = cellWidth, lastCellH = cellHeight;
        ShowImgui();

        if (lastCellW != cellWidth || lastCellH != cellHeight) {
            for (auto &ts : tileList) {
                // 设置position
                ts.spriteRenderer->transform.position = {
                    (ts.tileX + 0.5) * GetCellWidth(),
                    (ts.tileY + 0.5) * GetCellHeight()
                };
            }
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        if (ImGui::TreeNodeEx("RigidBody",
                              ImGuiTreeNodeFlags_Selected |
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
            rigidBody2D->ShowImgui();
            ImGui::TreePop();
        }
        ImGui::PopStyleColor();

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
            for (int i = 0; i < tileSetList.size(); ++i) {
                auto &ts = tileSetList[i];

                std::filesystem::path p = ts.texture->GetFilePath();
                auto textNameText = p.stem().string();

                std::string label = util::Concat("TileSet-", textNameText, "##", ts.texture->GetId());
                bool open = ImGui::TreeNodeEx(label.c_str());
                if (open) {
                    ImGui::Indent(5);

                    if (MyImGui::DrawButton("texture", textNameText)) {
                        FileSystemWindow::SetLocalPath(p.parent_path());
                    }
                    if (MyImGui::DrawIntSpinner("columns", ts.columns, 1)) {
                        ts.cellWidth = ts.texture->GetWidth() / ts.columns;
                    }
                    if (MyImGui::DrawIntSpinner("rows", ts.rows, 1)) {
                        ts.cellHeight = ts.texture->GetHeight() / ts.rows;
                    }

                    std::string treeLabel = util::Concat("tiles##" + label);
                    if (ImGui::TreeNodeEx(treeLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)){

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

                        //if (ImGui::BeginChild(util::Concat("tiles-child##", index).c_str())) {

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

                                ImVec2 uv0 = {leftX, topY};
                                ImVec2 uv1 = {rightX, bottomY};

                                bool selected = btnChoose == i && selectTsIndex == index;
                                if (selected)
                                    ImGui::PushStyleColor(ImGuiCol_Border, {0.95, 0.9, 0.15, 1});
                                ImGui::PushID(util::Concat("btn", label, "-", i).c_str());
                                if (ImGui::ImageButton(
                                    reinterpret_cast<ImTextureID>(
                                        ts.texture->GetId()),
                                    btnSize, uv0, uv1)) {
                                    selectTsIndex = index;

                                    if (!selected) {
                                        btnChoose = i;
                                        SetCursorTile(index, i);
                                        hasChosenTile = true;
                                    } else {
                                        btnChoose = -1;
                                        hasChosenTile = false;
                                    }
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
                            //ImGui::EndChild();
                        //}

                        ImGui::TreePop();
                    }
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.2, 0.2, 1});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.6, 0.1, 0.15, 1});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.7, 0.2, 0.2, 1});

                    ImGui::PushID(util::Concat("Delete##", index).c_str());
                    if (ImGui::Button("Delete", {ImGui::GetContentRegionAvail().x,
                                                 ImGui::GetTextLineHeight()+ImGui::GetStyle().FramePadding.y*2})) {
                        // TODO 删除tileSet
                        for (int j = i; j < tileSetList.size()-1; ++j) {
                            tileSetList[j] = tileSetList[j+1];
                        }
                        tileSetList.pop_back();
                    }
                    ImGui::PopID();

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

    cell.tileX = x;
    cell.tileY = y;
    cell.index = tileIndex;
    cell.tileSetIndex = tileSetIndex;

    TileSet &tileSet = tileSetList[tileSetIndex];
    cell.tileSetPtr = &tileSet;

    cell.spriteRenderer = new SpriteRenderer();
    auto spr = cell.spriteRenderer;
    spr->SetTexture(tileSet.texture);

    auto ani = spr->GetAnimation();
    ani->SetFrame(tileIndex);
    ani->SetVFrames(tileSet.rows);
    ani->SetHFrames(tileSet.columns);
    ani->ApplyModifyToSprite(spr->GetSprite());

    auto &tr = spr->transform;

    // 设置position
    tr.position = {
        (x + 0.5) * GetCellWidth(),
        (y + 0.5) * GetCellHeight()
    };

    // 设置scale，使其与cellSize吻合
    tr.scale = {
        tileSet.cellWidth / (tileSet.texture->GetWidth() / tileSet.columns),
        tileSet.cellHeight / (tileSet.texture->GetHeight() / tileSet.rows)
    };


    rigidBody2D->AddNode(spr); // 设置父节点为自身，让GetTransform和GetMatrix等正常工作

    // tileCell加入数组
    tileList.push_back(cell);

    // 添加碰撞体
    auto coll = spr->AddNode<ColliderShape2D>();
    auto box = new Box2DCollider(coll);
//    coll->transform.position = tr.position;
    box->SetSize({GetCellWidth(), GetCellHeight()});
    coll->SetCollider(box);

    if (MainWindow::GetScene()->IsRunning())
        spr->Start(); // 初始化节点
}

void TileMap::Update(float dt) {
    for (auto &t : tileList) {
        // 统一设置visitable
        if (t.spriteRenderer->IsVisitable() != active) {
            t.spriteRenderer->SetVisitable(active);
        }
    }

    if (!active) return;

    for (auto &t : tileList) {
        // 统一设置z-index
        if (zIndex.GetZIndex() != t.spriteRenderer->GetZIndex()) {
            t.spriteRenderer->SetZIndex(zIndex.GetZIndex());
        }
    }
    rigidBody2D->Update(dt);

    Node::Update(dt);
}

void TileMap::EditorUpdate(float dt) {
    for (auto &t : tileList) {
        // 统一设置visitable
        if (t.spriteRenderer->IsVisitable() != active) {
            t.spriteRenderer->SetVisitable(active);
        }
    }

    if (!active) return;

    if (PropertiesWindow::GetActiveNode() == this) {
        ShowTileMapGrids();
    }


    for (auto &t : tileList) {
        // 统一设置z-index
        if (zIndex.GetZIndex() != t.spriteRenderer->GetZIndex()) {
            t.spriteRenderer->SetZIndex(zIndex.GetZIndex());
        }
    }

    rigidBody2D->EditorUpdate(dt);

    // 世界转换为TileMap的坐标系
    auto pos = WorldPosToGridPos(MouseListener::GetWorldPos());
    int x = (int) pos.x, y = (int) pos.y;

    // TODO 将编辑操作拆分出来
    if (GameViewWindow::GetWantCaptureMouse() &&
        IsAncestorOrSelf(PropertiesWindow::GetActiveNode())) {
        // 右键删除
        if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
            static int lastX = x+1, lastY = y+1;
            if (lastX != x || lastY != y) {
                for (int i = 0; i < tileList.size(); ++i) {
                    auto &tc = tileList[i];
                    if (tc.tileX != x || tc.tileY != y)
                        continue;
                    delete tc.spriteRenderer;
                    for (int j = i; j < tileList.size() - 1; ++j) {
                        tileList[j] = tileList[j+1];
                    }
                    tileList.pop_back();
                    break;
                }

                lastX = x;
                lastY = y;
            }
        }

        if (hasChosenTile) {
            // 光标显示
            cursorTile.spriteRenderer->SetVisitable(true);

            if (zIndex.GetZIndex() !=
                cursorTile.spriteRenderer->GetZIndex()) {
                cursorTile.spriteRenderer->SetZIndex(zIndex.GetZIndex());
            }

            cursorTile.SetPosition(x, y);

            cursorTile.spriteRenderer->SetDirty();
            cursorTile.spriteRenderer->EditorUpdate(dt);

            // 左键绘画
            if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
                static int lastX = x+1, lastY = y+1;
                // 添加
                if (lastX != x || lastY != y) {
                    bool shouldAdd = true;
                    for (auto &tc : tileList) {
                        if (tc.tileX == x && tc.tileY == y) {
                            shouldAdd = false;
                            break;
                        }
                    }
                    if (shouldAdd) {
                        AddTileCell(x, y, cursorTile.tileSetIndex, cursorTile.index);
                    }

                    lastX = x;
                    lastY = y;
                }
            }
        }

        // esc取消绘画
        if (KeyListener::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            if (hasChosenTile) {
                hasChosenTile = false;
                cursorTile.spriteRenderer->SetVisitable(false);
            }
        }
    }
    // 鼠标不在视窗范围内，不显示光标
    else {
        if (hasChosenTile)
            cursorTile.spriteRenderer->SetVisitable(false);
    }

    Node::EditorUpdate(dt);
}

void TileMap::SetCursorTile(int tileSetIndex, int tileIndex) {
    if (tileSetIndex >= tileSetList.size()) return;

    TileCell &cell = cursorTile;
    cell.tileMapPtr = this;
    cell.index = tileIndex;
    cell.tileSetIndex = tileSetIndex;

    TileSet &tileSet = tileSetList[tileSetIndex];
    cell.tileSetPtr = &tileSet;

    bool first = false;
    if (!cell.spriteRenderer) {
        cell.spriteRenderer = new SpriteRenderer();
        first = true;
    }

    auto spr = cell.spriteRenderer;
    spr->SetTexture(tileSet.texture);

    auto ani = spr->GetAnimation();
    ani->SetFrame(tileIndex);
    ani->SetVFrames(tileSet.rows);
    ani->SetHFrames(tileSet.columns);
    ani->ApplyModifyToSprite(spr->GetSprite());

    auto &tr = spr->transform;

    // 设置scale，使其与cellSize吻合
    tr.scale = {
        tileSet.cellWidth / (tileSet.texture->GetWidth() / tileSet.columns),
        tileSet.cellHeight / (tileSet.texture->GetHeight() / tileSet.rows)
    };

    if (first) {
        // 设置position
        tr.position = {
            (1.5) * GetCellWidth() * GetTransform().scale.x,
            (1.5) * GetCellHeight() * GetTransform().scale.y
        };

        spr->Start(); // 将spriteRenderer加入渲染器
        spr->parent = rigidBody2D; // 设置父节点为自身，让GetTransform和GetMatrix等正常工作

        spr->SetColor({0.2, 0.2, 0.2, 0.2});
    }
}

void TileMap::ShowTileMapGrids() {
    int cellW = GetCellWidth();
    int cellH = GetCellHeight();

    if (cellW == 0 || cellH == 0) return;

    auto camera = MainWindow::GetScene()->GetCamera();
    auto cameraPos = camera->position;
    auto cameraZoom = camera->GetZoom();
    auto projSize = camera->GetProjectionSize();

    int mWH = cellW < cellH ? cellW : cellH;
    if (cameraZoom / mWH > 0.12) return;

    auto mat = GetModelMatrix();
    auto pos =  util::TransformPoint(parent->GetModelMatrix(), transform.position);
    cameraPos -= pos;

    // 计算视口边界
    float left = (cameraPos.x - (projSize.x * 0.5f * cameraZoom));
    float right = (cameraPos.x + (projSize.x * 0.5f * cameraZoom));
    float top = (cameraPos.y + (projSize.y * 0.5f * cameraZoom));
    float bottom = (cameraPos.y - (projSize.y * 0.5f * cameraZoom));

    // 绘制垂直网格线
    for (int i = (int)left - (int)left % cellW; i < (int)right; i += cellW) {
        glm::vec2 down = {i, bottom};
        glm::vec2 up = {i, top};
        down = util::TransformPoint(mat, down);
        up = util::TransformPoint(mat, up);
        DebugDraw::AddLine2D(up, down, {0.45, 0.35, 0.32, 1});
    }

    // 绘制水平网格线
    for (int i = (int)bottom - (int)bottom % cellH; i < (int)top; i += cellH) {
        glm::vec2 left_ = {left, i};
        glm::vec2 right_ = {right, i};
        left_ = util::TransformPoint(mat, left_);
        right_ = util::TransformPoint(mat, right_);
        DebugDraw::AddLine2D(left_, right_, {0.45, 0.35, 0.32, 1});
    }
}


glm::vec2 TileMap::WorldPosToGridPos(const glm::vec2 &wp) {
    auto pos = util::TransformPoint(glm::inverse(GetModelMatrix()), wp);
    pos = pos / glm::vec2(GetCellWidth(), GetCellHeight()) - glm::vec2(0.5, 0.5);
    int x = (int)round(pos.x);
    int y = (int)round(pos.y);
    return {x, y};
}

void TileMap::Start() {
    rigidBody2D->Start();
    Node::Start();
}

BEGIN_RTTR_REG(TileMap)
RTTR_CLASS(TileMap)
    .constructor<>()(
        rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )
    .property("cellWidth", &TileMap::GetCellWidth, &TileMap::SetCellWidth)
    .property("cellHeight", &TileMap::GetCellHeight, &TileMap::SetCellHeight);
END_RTTR_REG(TileMap)