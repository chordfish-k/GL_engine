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
    int columns, rows;
public:
    Spritesheet(Texture *texture, int spriteWidth, int spriteHeight,
                int numSprite);

    Sprite *GetSprite(unsigned int index);

    // 属性
    
    int Size() {
        return texCoordsEachSprite.size();
    }

    int GetColumns() {return columns;}

    int GetRows() {return rows;}
};