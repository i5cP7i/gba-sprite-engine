//
// Created by eduar on 12/23/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_ENEMY_H
#define GBA_SPRITE_ENGINE_PROJECT_ENEMY_H

#include "CharacterBase.h"
#include "SoldierRed.h"

class Enemy : public CharacterBase
{
private:

public:
    Enemy(std::string Name, int x, int y);
    void Reset() final;
    std::string GetName() const {return Name;}
    eDirection GetDirection() const { return CurrentDirection; }

};


#endif // GBA_SPRITE_ENGINE_PROJECT_ENEMY_H
