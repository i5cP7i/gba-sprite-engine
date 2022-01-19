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
            .withLocation(x+TileOffset.x, y+TileOffset.y)
            .buildPtr();
    CharacterSprite->flipHorizontally(false);
    CharacterDirection = eDirection::SouthWest;
    dx = 2;
    dy = 1;
    FrameOrientation = 2;
    PrevFrame = 0;

    TileSystem = std::unique_ptr<TileSystemBase>(new TileSystemBase());

    TileLocation.x = x;
    TileLocation.y = y;

    TileSystem->TileLocation.x = x;
    TileSystem->TileLocation.y = y;
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
    this->Direction = Direction;
    Update();
}

void CharacterBase::Move(TileSystemBase::TileCoordinates T)
{
    CharacterSprite->moveTo(T.x * 16 + TileOffset.x,T.y * 8 + TileOffset.y);
    TileLocation = T;
    TileSystem->TileLocation = {T.x * 16, T.y * 8};
}

void CharacterBase::Move(int x, int y)
{
    CharacterSprite->moveTo(x+TileOffset.x,y+TileOffset.y);
    TileLocation.x = x/32;
    TileLocation.y = y/16;
    TileSystem->TileLocation.x = x/32;
    TileSystem->TileLocation.y = y/16;
}

// If true, p is out of range.
bool CharacterBase::isOutOfRange(TileSystemBase::TileCoordinates t, int Radius)
{
    if ((t.x >= (TileSystem->WorldLocation.x-Radius) && (t.y >= (TileSystem->WorldLocation.y-Radius)))
    && (t.x >= (TileSystem->WorldLocation.x-Radius) && (t.y <= (TileSystem->WorldLocation.y+Radius)))
    && (t.x <= (TileSystem->WorldLocation.x+Radius) && (t.y >= (TileSystem->WorldLocation.y-Radius)))
    && (t.x <= (TileSystem->WorldLocation.x+Radius) && (t.y <= (TileSystem->WorldLocation.y+Radius))))
    {
        return false;
    }
    return true;
}



