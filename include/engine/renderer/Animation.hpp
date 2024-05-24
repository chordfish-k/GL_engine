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

    void ApplyModifyToSprite(Sprite *sprite);

    void Imgui();

    void Reset();

    json Serialize() override;

    Animation *Deserialize(json j) override;

    // 属性
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
