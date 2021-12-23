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

void Player::HandleMovement() {
    switch(CharacterDirection)
    {
        case eDirection::SouthEast: // KEY_RIGHT
            CharacterSprite->moveTo(CharacterSprite->getX()+2, CharacterSprite->getY()+1);
            break;
        case eDirection::SouthWest: // KEY_DOWN
            CharacterSprite->moveTo(CharacterSprite->getX()-2, CharacterSprite->getY()+1);
            break;
        case eDirection::NorthEast: // KEY_UP
            CharacterSprite->moveTo(CharacterSprite->getX()+2, CharacterSprite->getY()-1);
            break;
        case eDirection::NorthWest: // KEY_LEFT
            CharacterSprite->moveTo(CharacterSprite->getX()-2, CharacterSprite->getY()-1);
            break;
        default:
            break;
    }
}

void Player::SetDirection(eDirection Direction)
{
    switch(Direction)
    {
        case eDirection::SouthEast: // KEY_RIGHT
            CharacterSprite->flipHorizontally(true);
            FrameOrientation = 2;
            PrevFrame = 0;
            break;
        case eDirection::SouthWest: // KEY_DOWN
            CharacterSprite->flipHorizontally(false);
            FrameOrientation = 2;
            PrevFrame = 0;
            break;
        case eDirection::NorthEast: // KEY_UP
            CharacterSprite->flipHorizontally(true);
            FrameOrientation = 5;
            PrevFrame = 3;
            break;
        case eDirection::NorthWest: // KEY_LEFT
            CharacterSprite->flipHorizontally(false);
            FrameOrientation = 5;
            PrevFrame = 3;
            break;
        default:
            break;
    }
    CharacterSprite->animateToFrame(FrameOrientation-3);
    CharacterDirection = Direction;
    Update();
}

void Player::AnimateWalking()
{
    if (CharacterSprite->getCurrentFrame() == FrameOrientation && PrevFrame != FrameOrientation)
    {
        PrevFrame = CharacterSprite->getCurrentFrame();
        CharacterSprite->animateToFrame(FrameOrientation-2);
    }
    else if (CharacterSprite->getCurrentFrame() == FrameOrientation-1 && PrevFrame == FrameOrientation)
    {
        PrevFrame = CharacterSprite->getCurrentFrame();
        CharacterSprite->animateToFrame(FrameOrientation-3);
    }
    Update();
}
