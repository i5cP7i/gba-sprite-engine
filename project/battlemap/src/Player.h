//
// Created by eduar on 12/14/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_PLAYER_H
#define GBA_SPRITE_ENGINE_PROJECT_PLAYER_H

#include "CharacterBase.h"
#include "SoldierBlue.h"
#include <libgba-sprite-engine/gba_engine.h>

class Player : public CharacterBase
{
private:

public:
    Player();
    Player(int x, int y);

    eDirection GetDirection() const { return Direction; }
};


#endif //GBA_SPRITE_ENGINE_PROJECT_PLAYER_H
