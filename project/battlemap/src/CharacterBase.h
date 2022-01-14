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
    // int TileIndex = GetTileIndex();
    TileSystemBase::TileCoordinates TileLocation;
public:

    enum class eDirection: unsigned char { SouthEast = 0, SouthWest = 1, NorthEast = 2, NorthWest = 3 } Direction;
    CharacterBase(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames);

    void Move(int x, int y);

    void HandleMovement();
    void AnimateWalking();
    void AnimateHalt();

    void SetDirection(eDirection Direction);
    eDirection GetDirection() const { return Direction; }
    TileSystemBase::TileCoordinates GetTileLocation() const { return TileLocation; }

    bool isOutofRange(TileSystemBase::TileCoordinates t, int Radius); // t = TileCoordinates, p = TileCoordinates
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

    // Character Attributes
    int MaxHealth;
    int Health;
    int MoveRadius;
    int AttackRadius;
    int Strength;
    int Evasion;

    int ClipValue(int Number, int LowerBound, int UpperBound)
    {
        return std::max(LowerBound, std::min(Number, UpperBound));
    }
public:
    int GetHealth() const { return Health; }
    void SetHealth(int Health)  { this->Health = ClipValue(Health, 0, MaxHealth); }
    int GetMoveRadius() const { return MoveRadius; }
    int GetAttackRadius() const { return AttackRadius; }
    int GetStrength() const { return Strength; }
    int GetEvasion() const { return Evasion; }
};


#endif //GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
