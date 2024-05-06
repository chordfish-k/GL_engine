#include "engine/node/EditorSpriteGizmo.hpp"
#include "engine/node/SpriteRenderer.hpp"
#include "engine/editor/PropertiesWindow.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/renderer/Rect.hpp"
#include "engine/core/MouseListener.hpp"
#include "engine/editor/GameViewWindow.hpp"

void EditorSpriteGizmo::Update(float dt) {
    Node *activeNode = PropertiesWindow::GetActiveNode();
    if (activeNode == nullptr || activeNode->GetNodeType() != "SpriteRenderer") return;
    auto *spr = (SpriteRenderer*)activeNode;
    Transform t = spr->GetTransform();

    // 画外包围盒
    auto scale = t.scale;
    auto position = t.position;
    auto d = spr->GetSize() * 0.5f;// * scale;
    auto center = spr->IsCentered() ? glm::vec2(0, 0) : d;
    auto pos = spr->GetModelMatrix() * (glm::vec4(center.x, center.y, 0, 1));
    auto dim = d * scale;
    auto posi = glm::vec2(pos.x, pos.y);
    DebugDraw::AddBox2D(posi, dim, t.rotation);

    if (!GameViewWindow::GetWantCaptureMouse()) return;


    Rect mainRect(posi-dim, dim*2.f);
//    util::Println(mainRect.x, ", ", mainRect.y, ", ", mainRect.w, ", ", mainRect.h);



    static bool dragging = false;

    if (MouseListener::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        if (!dragging) {
            glm::vec2 mousePos = MouseListener::GetWorldPos();
            dragging = mainRect.IsInRect(mousePos);
        }
        auto dp = glm::vec2(MouseListener::GetWorldDx(), MouseListener::GetWorldDy());
        activeNode->transform.position += dp / (scale / activeNode->transform.scale);

        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
    }
    else {
        dragging = false;
    }

    Node::Update(dt);
}
