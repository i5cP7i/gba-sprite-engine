//
// Created by eduar on 12/14/2021.
//

#include "Player.h"

Player::Player()
        : CharacterBase(soldierTiles, sizeof(soldierTiles), 80, 102, 30, 24)
{

}

Player::Player(int x, int y)
        : CharacterBase(soldierTiles, sizeof(soldierTiles), x, y, 30, 24)
{

}