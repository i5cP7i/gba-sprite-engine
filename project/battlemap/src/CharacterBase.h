//
// Created by eduar on 12/10/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include "TileSystemBase.h"

class CharacterBase
{
private:

public:
    enum class eDirection: unsigned char {SouthEast, SouthWest, NorthEast, NorthWest} Direction;
    CharacterBase(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames);

    void Move(eDirection Direction, int NumTiles);

    void HandleMovement();
    void AnimateWalking();

    void SetDirection(eDirection Direction);

    void Update() const { CharacterSprite->update(); }
    Sprite* Get() const { return CharacterSprite.get(); }

protected:
    SpriteBuilder<Sprite> CharacterBuilder;
    std::unique_ptr<Sprite> CharacterSprite;
    u32 PrevFrame = 0;
    u32 FrameOrientation = 2;
    eDirection CharacterDirection;
    int AnimationDelay;
    int AnimationFrames;
    int x;
    int y;
    int dx;
    int dy;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
