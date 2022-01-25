//
// Created by eduar on 12/28/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/palette/palette_manager.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <vector>
#include <memory>

#include "TileSelectionData.h"
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
    struct TileCoordinates
    {
        int x;
        int y;
        void operator += (TileCoordinates v)
        {
            x += v.x;
            y += v.y;
        }

        void operator -= (TileCoordinates v)
        {
            x -= v.x;
            y -= v.y;
        }
        TileCoordinates operator - (TileCoordinates v)
        {
            return
            {
                -v.x,
                -v.y,
            };
        }
        TileCoordinates operator + (TileCoordinates v)
        {
            return
            {
                    +v.x,
                    +v.y,
            };
        }
        void operator = (TileCoordinates v)
        {
            x = v.x;
            y = v.y;
        }
        bool operator == (TileCoordinates v)
        {
            return (x == v.x) && (y == v.y);
        }
    };
    const int TileHeight = 16;
    const int TileWidth = 32;

    TileCoordinates TileLocation; // tile i = TileY * WIDTH + TileX
    TileCoordinates WorldSize = { 16, 16 };
    TileCoordinates TileSize = { TileWidth/2, TileHeight/2};
    TileCoordinates WorldOrigin = {0, 0};
    TileCoordinates WorldLocation = {0, 0};
    TileCoordinates WorldOffset = {0,0};
    TileCoordinates WorldCartesianLocation;



    // int x, y;

    int ResetX = 240;
    int ResetY = 180;

    enum class eStatus {Valid, Invalid, Inactive};
    eStatus TileStatus;
    TileSystemBase();

    TileCoordinates GetTileLocation() const { return TileLocation; }
    TileCoordinates GetWorldLocation() const { return WorldLocation; }
    TileCoordinates GetWorldTransform(TileCoordinates T) const { return {(T.x) / TileSize.x, (T.y) / TileSize.y}; }
    TileCoordinates GetWorldTransformOffset(TileCoordinates T) const { return {(T.x%TileSize.x), (T.y%TileSize.y)}; }
    TileCoordinates GetWorldCartesian() const;
    void SetWorldOffset(TileCoordinates T) { WorldOffset = T; }

    void SetTileStatus(eStatus Status);
    eStatus GetTileStatus() const { return TileStatus; }
    void Update();
    void UpdateLocation();
    void Move(int x, int y);
    void Move(TileCoordinates T);
    void MoveRelative(TileCoordinates T);
    void ResetPos()
    {
        for (int i = 0; i < TileSelectionSpriteVector.size() / 2; i += 2)
        {
            TileSelectionSpriteVector.at(i)->moveTo(ResetX, ResetY);
            TileSelectionSpriteVector.at(i+1)->moveTo(ResetX+TileWidth/2, ResetY);
        }
        UpdateLocation();
    }
    void MoveRight();
    void MoveLeft();
    void MoveUp();
    void MoveDown();

    std::vector<Sprite*> Get() const;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_TILESYSTEMBASE_H
