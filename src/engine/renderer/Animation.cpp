#include "engine/renderer/Animation.hpp"

void Animation::ApplyModifyToSprite(Sprite *sprite) {
    if (modified) {
        float W = sprite->GetTexture()->GetWidth();
        float H = sprite->GetTexture()->GetHeight();
        float spriteW = W / hFrames;
        float spriteH = H / vFrames;
        sprite->SetWidth(spriteW);
        sprite->SetHeight(spriteH);
        int i = hFrames -
                (frame / hFrames) - 1;
        int j = frame % hFrames;
        // 将坐标缩到[0,1]区间，作为texCoords
        float topY = (i + 1) * spriteH / H;
        float rightX = (j + 1) * spriteW / W;
        float leftX = j * spriteW / W;
        float bottomY = i * spriteH / H;

        sprite->SetTexCoords({
            glm::vec2(rightX, topY),
            glm::vec2(rightX, bottomY),
            glm::vec2(leftX, bottomY),
            glm::vec2(leftX, topY),
        });
        modified = false;
    }
}

bool Animation::Imgui() {
    bool res = false;
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (ImGui::TreeNodeEx("Animation", ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen)) {
        int hFrames_ = GetHFrames();
        int vFrames_ = GetVFrames();
        int frame_ = GetFrame();

        if (MyImGui::DrawIntSpinner("HFrames", hFrames_, 1)) {
            SetHFrames(hFrames_);
            res = true;
        }
        if (MyImGui::DrawIntSpinner("VFrames", vFrames_, 1)) {
            SetVFrames(vFrames_);
            res = true;
        }
        if (MyImGui::DrawIntSpinner("Frame", frame_,0)) {
            SetFrame(frame_);
            res = true;
        }

        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    return res;
}

void Animation::Reset() {
    hFrames = vFrames = 1;
    frame = 0;
}

json Animation::Serialize() {
    json j;
    j["hFrames"] = hFrames;
    j["vFrames"] = vFrames;
    j["frame"] = frame;
    return j;
}

Animation *Animation::Deserialize(json j) {
    auto &h = j["hFrames"];
    auto &v = j["vFrames"];
    auto &f = j["frame"];
    if (!h.empty())
        SetHFrames(h);
    if (!v.empty())
        SetVFrames(v);
    if (!f.empty())
        SetFrame(f);
    return this;
}