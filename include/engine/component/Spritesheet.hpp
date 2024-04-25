#pragma once

#include "engine/component/Sprite.hpp"
#include "engine/renderer/Texture.hpp"
#include <vector>

class Spritesheet {
private:
    Texture *texture;
    std::vector<Sprite *> sprites;

public:
    Spritesheet(Texture *texture, int spriteWidth, int spriteHeight,
                int numSprite, int spacing);

    Sprite *GetSprite(unsigned int index);
};