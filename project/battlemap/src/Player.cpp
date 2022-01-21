//
// Created by eduar on 12/14/2021.
//

#include "Player.h"

Player::Player(std::string Name, int x, int y)
        : CharacterBase(soldierBlueTiles, sizeof(soldierBlueTiles), x, y, 40, 24)
{
    this->Name = Name;
    MaxHealth = 102;
    Health = MaxHealth;
    MoveRadius = 4;
    AttackRadius = 1;
    Strength = 20;
    Evasion = 10;
}


void Player::Reset()
{
    MaxHealth = 102;
    Health = MaxHealth;
    MoveRadius = 4;
    AttackRadius = 1;
    Strength = 20; // Default = 20
    Evasion = 10;

    Move(3*TileSystem->TileWidth/2,  13*TileSystem->TileHeight/2);
    TileSystem->Move(3*TileSystem->TileWidth/2, 13*TileSystem->TileHeight/2);
    SetDirection(CharacterBase::eDirection::SouthWest);
}
