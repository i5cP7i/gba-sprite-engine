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
    Player(std::string Name, int x, int y);
    void Reset() final;

    std::string GetName() const {return Name;}
    eDirection GetDirection() const { return Direction; }
};


#endif //GBA_SPRITE_ENGINE_PROJECT_PLAYER_H
