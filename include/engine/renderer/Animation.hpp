#pragma once

#include <imgui.h>
#include "engine/editor/MyImGui.hpp"
#include "Sprite.hpp"

class SpriteRenderer;

class Animation : public ASerializableObj{
private:
    int hFrames, vFrames;
    int frame;
    bool modified = false;
public:
    friend class SpriteRenderer;
    Animation() :hFrames(1), vFrames(1), frame(0){}

    void ApplyModifyToSprite(Sprite *sprite) {
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

    void Imgui() {
        ImGui::SetNextItemOpen(true);
        if (ImGui::TreeNode("Animation")) {
            int hFrames_ = GetHFrames();
            int vFrames_ = GetVFrames();
            int frame_ = GetFrame();

            if (MyImGui::DrawIntSpinner("HFrames", hFrames_, 1)) {
                SetHFrames(hFrames_);
            }
            if (MyImGui::DrawIntSpinner("VFrames", vFrames_, 1)) {
                SetVFrames(vFrames_);
            }
            if (MyImGui::DrawIntSpinner("Frame", frame_,0)) {
                SetFrame(frame_);
            }

            ImGui::TreePop();
        }
    }

    json Serialize() override {
        json j;
        j["hFrames"] = hFrames;
        j["vFrames"] = vFrames;
        j["frame"] = frame;
        return j;
    }

    Animation *Deserialize(json j) override {
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

    int GetHFrames() const { return hFrames; }
    void SetHFrames(int hFrames) {
        if (hFrames <= 0) return;
        this->hFrames = hFrames;
        this->modified = true;
    }
    int GetVFrames() const { return vFrames; }
    void SetVFrames(int vFrames) {
        if (vFrames <= 0) return;
        this->vFrames = vFrames;
        this->modified = true;
    }
    int GetFrame() const { return frame; }
    void SetFrame(int frame) {
        if (vFrames < 0) return;
        this->frame = frame;
        this->modified = true;
    }
    bool IsModified() const { return modified; }
};
