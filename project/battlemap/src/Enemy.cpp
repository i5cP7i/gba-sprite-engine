//
// Created by eduar on 12/23/2021.
//

#include "Enemy.h"

Enemy::Enemy()
        : CharacterBase(soldierRedTiles, sizeof(soldierRedTiles), 80, 102, 30, 24)
{
    int Health = 86;
    int MoveRadius = 3;
    int Strength = 7;
    int Evasion = 5;
}

Enemy::Enemy(int x, int y)
        : CharacterBase(soldierRedTiles, sizeof(soldierRedTiles), x, y, 30, 24)
{
    int Health = 86;
    int MoveRadius = 3;
    int Strength = 7;
    int Evasion = 5;
}

