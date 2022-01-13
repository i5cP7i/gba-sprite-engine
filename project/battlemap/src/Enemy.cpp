//
// Created by eduar on 12/23/2021.
//

#include "Enemy.h"

Enemy::Enemy()
        : CharacterBase(soldierRedTiles, sizeof(soldierRedTiles), 80, 102, 30, 24)
{
    MaxHealth = 86;
    Health = MaxHealth;
    MoveRadius = 4;
    Strength = 7;
    Evasion = 5;
}

Enemy::Enemy(int x, int y)
        : CharacterBase(soldierRedTiles, sizeof(soldierRedTiles), x, y, 30, 24)
{
    MaxHealth = 86;
    Health = MaxHealth;
    MoveRadius = 4;
    Strength = 7;
    Evasion = 5;

}

