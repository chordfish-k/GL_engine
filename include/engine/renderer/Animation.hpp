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

    Animation(int hFrames, int vFrames, int frame)
        :hFrames(hFrames), vFrames(vFrames), frame(frame){}

    void ApplyModifyToSprite(Sprite *sprite);

    bool Imgui();

    void Reset();

    json Serialize() override;

    Animation *Deserialize(json j) override;

    // 属性
    int GetHFrames();
    void SetHFrames(int hFrames);
    int GetVFrames();
    void SetVFrames(int vFrames);
    int GetFrame();
    void SetFrame(int frame);
    bool IsModified();
    void ClearModify();

};
