//
// Created by eduar on 12/23/2021.
//

#include "Enemy.h"

Enemy::Enemy(int x, int y)
        : CharacterBase(soldierRedTiles, sizeof(soldierRedTiles), x, y, 40, 24)
{
    MaxHealth = 86;
    Health = MaxHealth;
    MoveRadius = 4;
    AttackRadius = 1;
    Strength = 250;
    Evasion = 5;
}

void Enemy::Reset()
{
    MaxHealth = 86;
    Health = MaxHealth;
    MoveRadius = 4;
    AttackRadius = 1;
    Strength = 250; // Default = 25
    Evasion = 5;
    Move(13*TileSystem->TileWidth/2, 13*TileSystem->TileHeight/2);
    TileSystem->Move(13*TileSystem->TileWidth/2, 13*TileSystem->TileHeight/2);

}

