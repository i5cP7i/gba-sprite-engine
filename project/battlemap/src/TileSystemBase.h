//
// Created by eduar on 12/28/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/palette/palette_manager.h>
#include <vector>
#include <memory>

#include "TileSelectionData.h"
#include "CharacterBase.h"
#include "Plains.h"

class TileSystemBase
{
private:
    SpriteBuilder<Sprite> TileSelectionBuilder; // Tile delta_x = 16, delta_y = 8
    std::vector<std::unique_ptr<Sprite>> TileSelectionSpriteVector;


    const unsigned short TileColorSet[14] =
    {
        0x7860,0x78C0,0x7941,0x7DC1,0x7E42,0x7EC3,0x7DC1,
        0x00FF,0x00FC,0x00DB,0x0DA,0x00DB,0x00DA,0x00D9,
    };

    unsigned short TileColors[7] = { 0 };

    unsigned short ColorShift = 0;
    unsigned ColorShiftTimer = 0;
    void ShiftColor();
public:
    int x, y;
    enum class eStatus {Valid, Invalid, Inactive};
    eStatus TileStatus;
    TileSystemBase();
    void SetTileStatus(eStatus Status);
    void Update();
    void Move(int x, int y);
    void MoveRight(int offsetX, int offsetY);
    void MoveLeft(int offsetX, int offsetY);
    void MoveUp(int offsetX, int offsetY);
    void MoveDown(int offsetX, int offsetY);

    std::vector<Sprite*> Get() const;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H