#include "engine/renderer/Spritesheet.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>

Spritesheet::Spritesheet(Texture *texture, int spriteWidth, int spriteHeight,
                         int numSprite, int spacing)
    : texture(texture), spriteWidth(spriteWidth), spriteHeight(spriteHeight){
    // 当前坐标指针，初始化取左上角第一格的左下角
    int currentX = 0;
    int currentY = texture->GetHeight() - spriteHeight;
    // 创建numSprites个sprite;
    for (int i = 0; i < numSprite; i++) {
        // 将坐标缩到[0,1]区间，作为texCoords
        float topY = (currentY + spriteHeight) / (float)texture->GetHeight();
        float rightX = (currentX + spriteWidth) / (float)texture->GetWidth();
        float leftX = currentX / (float)texture->GetWidth();
        float bottomY = currentY / (float)texture->GetHeight();

        std::vector<glm::vec2> texCoords = {
            glm::vec2(rightX, topY),
            glm::vec2(rightX, bottomY),
            glm::vec2(leftX, bottomY),
            glm::vec2(leftX, topY),
        };
//        Sprite *sprite = new Sprite();
//        sprite->SetTexture(texture);
//        sprite->SetWidth(spriteWidth);
//        sprite->SetHeight(spriteHeight);
//        sprite->SetTexCoords(texCoords);
//        sprites.push_back(sprite);
        texCoordsEachSprite.push_back(texCoords);
        // 移动当前坐标指针
        currentX += spriteHeight + spacing;
        if (currentX >= texture->GetWidth()) {
            currentX = 0;
            currentY -= spriteHeight + spacing;
        }
    }
}

Sprite *Spritesheet::GetSprite(unsigned int index) {
    Sprite *sprite = new Sprite();
            sprite->SetTexture(texture);
            sprite->SetWidth(spriteWidth);
            sprite->SetHeight(spriteHeight);
            sprite->SetTexCoords(texCoordsEachSprite[index]);
    return sprite;
}