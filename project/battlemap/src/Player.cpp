//
// Created by eduar on 12/14/2021.
//

#include "Player.h"

Player::Player(int x, int y)
        : CharacterBase(soldierBlueTiles, sizeof(soldierBlueTiles), x, y, 30, 24)
{
    MaxHealth = 102;
    Health = MaxHealth;
    MoveRadius = 3;
    AttackRadius = 1;
    Strength = 200;
    Evasion = 10;
}


void Player::Reset()
{
    MaxHealth = 102;
    Health = MaxHealth;
    MoveRadius = 3;
    AttackRadius = 1;
    Strength = 200; // Default = 20
    Evasion = 10;

    Move(3*TileSystem->TileWidth/2,  13*TileSystem->TileHeight/2);
    TileSystem->Move(3*TileSystem->TileWidth/2, 13*TileSystem->TileHeight/2);
}
