//
// Created by eduar on 12/14/2021.
//

#include "Player.h"
#include "Soldier.h"

Player::Player(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames)
    : PlayerDirection(eDirection::SouthWest), dx(2), dy(1)
{
    PlayerCharacterSprite = PlayerCharacterBuilder
            .withData(ImageData, ImageSize)
            .withSize(SIZE_16_32)
            .withAnimated(AnimationFrames, AnimationDelay)
            .withLocation(x, y)
            .buildPtr();
    PlayerCharacterSprite->flipHorizontally(false);
    PlayerFrameOrientation = 2;
    PlayerPrevFrame = 0;
}

void Player::HandleMovement() {
    switch(PlayerDirection)
    {
        case eDirection::SouthEast: // KEY_RIGHT
            PlayerCharacterSprite->moveTo(PlayerCharacterSprite->getX()+2, PlayerCharacterSprite->getY()+1);
            break;
        case eDirection::SouthWest: // KEY_DOWN
            PlayerCharacterSprite->moveTo(PlayerCharacterSprite->getX()-2, PlayerCharacterSprite->getY()+1);
            break;
        case eDirection::NorthEast: // KEY_UP
            PlayerCharacterSprite->moveTo(PlayerCharacterSprite->getX()+2, PlayerCharacterSprite->getY()-1);
            break;
        case eDirection::NorthWest: // KEY_LEFT
            PlayerCharacterSprite->moveTo(PlayerCharacterSprite->getX()-2, PlayerCharacterSprite->getY()-1);
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
            PlayerCharacterSprite->flipHorizontally(true);
            PlayerFrameOrientation = 2;
            PlayerPrevFrame = 0;
            break;
        case eDirection::SouthWest: // KEY_DOWN
            PlayerCharacterSprite->flipHorizontally(false);
            PlayerFrameOrientation = 2;
            PlayerPrevFrame = 0;
            break;
        case eDirection::NorthEast: // KEY_UP
            PlayerCharacterSprite->flipHorizontally(true);
            PlayerFrameOrientation = 5;
            PlayerPrevFrame = 3;
            break;
        case eDirection::NorthWest: // KEY_LEFT
            PlayerCharacterSprite->flipHorizontally(false);
            PlayerFrameOrientation = 5;
            PlayerPrevFrame = 3;
            break;
        default:
            break;
    }
    PlayerCharacterSprite->animateToFrame(PlayerFrameOrientation-3);
    PlayerDirection = Direction;
    Update();
}

void Player::AnimateWalking()
{
    if (PlayerCharacterSprite->getCurrentFrame() == PlayerFrameOrientation && PlayerPrevFrame != PlayerFrameOrientation)
    {
        PlayerPrevFrame = PlayerCharacterSprite->getCurrentFrame();
        PlayerCharacterSprite->animateToFrame(PlayerFrameOrientation-2);
    }
    else if (PlayerCharacterSprite->getCurrentFrame() == PlayerFrameOrientation-1 && PlayerPrevFrame == PlayerFrameOrientation)
    {
        PlayerPrevFrame = PlayerCharacterSprite->getCurrentFrame();
        PlayerCharacterSprite->animateToFrame(PlayerFrameOrientation-3);
    }
    Update();
}
