//
// Created by eduar on 12/14/2021.
//

#include "Player.h"

Player::Player()
        : CharacterBase(soldierBlueTiles, sizeof(soldierBlueTiles), 78, 102, 30, 24)
{

}

Player::Player(int x, int y)
        : CharacterBase(soldierBlueTiles, sizeof(soldierBlueTiles), x, y, 30, 24)
{

}