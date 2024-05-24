#include "engine/node/EditorGridLines.hpp"
#include "engine/core/MainWindow.hpp"

void EditorGridLines::EditorUpdate(float dt) {
    Node::Update(dt);

    auto camera = MainWindow::GetScene()->GetCamera();
    auto cameraPos = camera->position;
    auto cameraZoom = camera->GetZoom();
    auto projSize = camera->GetProjectionSize();

//    glm::vec2 center = {((int)projSize.x >> 1), ((int)projSize.y >> 1)};
//    glm::vec2 down = {center.x, cameraPos.y};
//    glm::vec2 up = {center.x, (int)(projSize.y * cameraZoom + cameraPos.y)};
//    glm::vec2 left = {cameraPos.x, center.y};
//    glm::vec2 right = {(int)(projSize.x * cameraZoom + cameraPos.x), center.y};

    glm::vec2 center = {0, 0};
    glm::vec2 down = {center.x, (int)(projSize.y * cameraZoom * -.5f + cameraPos.y)};
    glm::vec2 up = {center.x, (int)(projSize.y * cameraZoom * .5f + cameraPos.y)};
    glm::vec2 left = {(int)(projSize.x * cameraZoom * -.5f + cameraPos.x), center.y};
    glm::vec2 right = {(int)(projSize.x * cameraZoom * .5f + cameraPos.x), center.y};

    DebugDraw::AddLine2D(center, up, {0, 0.8, 0});
    DebugDraw::AddLine2D(center, down, {0.3, 0.8, 0.3});
    DebugDraw::AddLine2D(center, left, {0.8, 0, 0});
    DebugDraw::AddLine2D(center, right, {0.8, 0.3, 0.3});
}