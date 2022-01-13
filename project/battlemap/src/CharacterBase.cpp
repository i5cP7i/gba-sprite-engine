//
// Created by eduar on 12/10/2021.
//

#include "CharacterBase.h"

CharacterBase::CharacterBase(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames)
{
    CharacterSprite = CharacterBuilder
            .withData(ImageData, ImageSize)
            .withSize(SIZE_16_32)
            .withAnimated(AnimationFrames, AnimationDelay)
            .withLocation(x, y)
            .buildPtr();
    CharacterSprite->flipHorizontally(false);
    CharacterDirection = eDirection::SouthWest;
    dx = 2;
    dy = 1;
    FrameOrientation = 2;
    PrevFrame = 0;

    TileLocation.x = x;
    TileLocation.y = y;
}

void CharacterBase::HandleMovement()
{
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

void CharacterBase::AnimateWalking()
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

void CharacterBase::AnimateHalt()
{
    CharacterSprite->animateToFrame(0);
}

void CharacterBase::SetDirection(CharacterBase::eDirection Direction)
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

void CharacterBase::Move(int x, int y)
{
    CharacterSprite->moveTo(x, y);
    TileLocation.x = x;
    TileLocation.y = y;
}

// If true, p is out of range.
bool CharacterBase::isOutofRange(TileSystemBase::TileCoordinates t, int r)
{
    if ((t.x > (TileLocation.x + 16*r) && t.y > (TileLocation.y + 8*r)) ||
        (t.x < (TileLocation.x - 16*r) && t.y < (TileLocation.y - 8*r)) ||
        (t.x < (TileLocation.x - 16*r) && t.y > (TileLocation.y + 8*r)) ||
        (t.x > (TileLocation.x + 16*r) && t.y < (TileLocation.y - 8*r)))
    {
        return true;
    }
    return false;
}



