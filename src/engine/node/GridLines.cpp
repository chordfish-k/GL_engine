#include "engine/node/GridLines.hpp"
void GridLines::Update(float dt) {
    Node::Update(dt);

    auto camera = Window::GetScene()->GetCamera();
    auto cameraPos = camera->position;
    auto projSize = camera->GetProjectionSize();

    glm::vec2 center = {((int)projSize.x >> 1), ((int)projSize.y >> 1)};
    glm::vec2 down = {((int)projSize.x >> 1), 0};
    glm::vec2 up = {((int)projSize.x >> 1), (int)projSize.y};
    glm::vec2 left = {0, ((int)projSize.y >> 1)};
    glm::vec2 right = {(int)projSize.x, ((int)projSize.y >> 1)};

    DebugDraw::AddLine2D(center, up, {0, 0.8, 0});
    DebugDraw::AddLine2D(center, down, {0.3, 0.8, 0.3});
    DebugDraw::AddLine2D(center, left, {0.8, 0, 0});
    DebugDraw::AddLine2D(center, right, {0.8, 0.3, 0.3});
}