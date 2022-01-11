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

class TileSystemBase
{
private:

    SpriteBuilder<Sprite> TileSelectionBuilder; // Tile delta_x = 16, delta_y = 8
    std::vector<std::unique_ptr<Sprite>> TileSelectionSpriteVector;

    int x, y;

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
    enum class eStatus {Valid, Invalid, Inactive};
    eStatus TileStatus;
    TileSystemBase();
    void SetTileStatus(eStatus Status);
    void Update();
    void Move(int x, int y);
    void MoveRight();
    void MoveLeft();
    void MoveUp();
    void MoveDown();

    std::vector<Sprite*> Get() const;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H
