//
// Created by eduar on 12/10/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include "ObjectBase.h"
#include "TileSystemBase.h"

class CharacterBase
{
private:
    // int TileIndex = GetTileIndex();
    TileSystemBase::TileCoordinates TileOffset = {8, -19};

    TileSystemBase::TileCoordinates TileLocation;
    TileSystemBase::TileCoordinates WorldLocation;
public:
    std::unique_ptr<TileSystemBase> TileSystem;
    enum class eDirection: signed char { SouthEast = 2, SouthWest = 1, NorthEast = -1, NorthWest = -2 } Direction;
    enum class eAnimation: unsigned char { Walking = 0, Attacking = 1, Healing = 2 } Animation;
    CharacterBase(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames);

    void Move(int x,int y);
    void Move(TileSystemBase::TileCoordinates T);
    void MoveRelative(TileSystemBase::TileCoordinates T);

    void HandleMovement();
    void AnimateWalking();
    void AnimateHalt();
    bool AnimateAttack();

    void SetDirection(eDirection Direction);
    void SetAnimation(eAnimation Animation);
    eAnimation GetAnimation() const { return  Animation; }
    eDirection GetDirection() const { return Direction; }
    TileSystemBase::TileCoordinates GetTileLocation() const { return TileLocation; }

    bool isOutOfRange(TileSystemBase::TileCoordinates Target, int Radius); // t = TileCoordinates, p = TileCoordinates
    void Update() const { CharacterSprite->update(); Weapon->Get()->update(); }
    Sprite* Get() const { return CharacterSprite.get(); }
    Sprite* GetWeaponSprite() const { return Weapon->Get(); }

protected:
    std::unique_ptr<ObjectBase> Weapon;
    SpriteBuilder<Sprite> CharacterBuilder;
    std::unique_ptr<Sprite> CharacterSprite;
    u32 PrevFrame = 0;
    u32 FrameOrientation = 2;
    eDirection CurrentDirection;

    int AnimationDelay;
    int AnimationFrames;
    int dx;
    int dy;

    // Character Attributes
    std::string Name;
    int MaxHealth;
    int Health;
    int MoveRadius;
    int AttackRadius;
    int Strength;
    int Evasion;

    virtual void Reset() = 0;

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
