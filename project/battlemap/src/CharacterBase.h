//
// Created by eduar on 12/10/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

class CharacterBase
{
private:
    std::unique_ptr<Sprite> CharacterSprite;
public:
    CharacterBase();
    enum class eDirection: unsigned char {SouthEast, SouthWest, NorthEast, NorthWest} Direction;

protected:

    void Move(eDirection Direction, int NumTiles);
};


#endif //GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
