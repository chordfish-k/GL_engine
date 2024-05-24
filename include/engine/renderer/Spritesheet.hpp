#pragma once

#include "Sprite.hpp"
#include "engine/renderer/Texture.hpp"
#include <vector>

class Spritesheet {
private:
    Texture *texture;
//    std::vector<Sprite *> sprites;
    std::vector<std::vector<glm::vec2>> texCoordsEachSprite;
    int spriteWidth, spriteHeight;
public:
    Spritesheet(Texture *texture, int spriteWidth, int spriteHeight,
                int numSprite, int spacing);

    Sprite *GetSprite(unsigned int index);

    // 属性
    
    int Size() {
        return texCoordsEachSprite.size();
    }
};