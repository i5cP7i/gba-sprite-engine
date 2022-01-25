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
    CurrentDirection = eDirection::SouthWest;
    dx = 2;
    dy = 1;
    FrameOrientation = 2;
    PrevFrame = 0;

    TileSystem = std::unique_ptr<TileSystemBase>(new TileSystemBase());

    TileLocation.x = x/16;
    TileLocation.y = y/8;

    TileSystem->TileLocation.x = x;
    TileSystem->TileLocation.y = y;

    Animation = eAnimation::Walking;
}

void CharacterBase::HandleMovement()
{
    switch(CurrentDirection)
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
    if (Animation == eAnimation::Walking)
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

}

void CharacterBase::AnimateHalt()
{
    CharacterSprite->animateToFrame(0);
}

bool CharacterBase::AnimateAttack()
{
    if (Animation == eAnimation::Attacking)
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
        if (CharacterSprite->getCurrentFrame() == 23)
        {
            Update();
            return true;
        }
        return false;
    }
    else
    {
        Update();
        return false;
    }
}

void CharacterBase::SetDirection(CharacterBase::eDirection Direction)
{
    switch(Direction)
    {
        case eDirection::SouthEast: // KEY_RIGHT
            CharacterSprite->flipHorizontally(true);
            if (Animation == eAnimation::Walking)
            {
                FrameOrientation = 2;
                PrevFrame = 0;
            }
            else if (Animation == eAnimation::Attacking)
            {
                FrameOrientation = 23;
                PrevFrame = 21;
            }

            break;
        case eDirection::SouthWest: // KEY_DOWN
            CharacterSprite->flipHorizontally(false);
            if (Animation == eAnimation::Walking)
            {
                FrameOrientation = 2;
                PrevFrame = 0;
            }
            else if (Animation == eAnimation::Attacking)
            {
                FrameOrientation = 23;
                PrevFrame = 21;
            }
            break;
        case eDirection::NorthEast: // KEY_UP
            CharacterSprite->flipHorizontally(true);
            if (Animation == eAnimation::Walking)
            {
                FrameOrientation = 5;
                PrevFrame = 3;
            }
            else if (Animation == eAnimation::Attacking)
            {
                FrameOrientation = 25;
                PrevFrame = 23;
            }
            break;
        case eDirection::NorthWest: // KEY_LEFT
            CharacterSprite->flipHorizontally(false);
            if (Animation == eAnimation::Walking)
            {
                FrameOrientation = 5;
                PrevFrame = 3;
            }
            else if (Animation == eAnimation::Attacking)
            {
                FrameOrientation = 25;
                PrevFrame = 23;
            }
            break;
        default:
            break;
    }
    TextStream::instance().setText("FO:" + std::to_string(FrameOrientation),2,2);
    TextStream::instance().setText("PF:" + std::to_string(PrevFrame),3,2);
    CharacterSprite->animateToFrame(FrameOrientation-3);
    CurrentDirection = Direction;
    this->Direction = Direction;
    Update();
}

void CharacterBase::MoveRelative(TileSystemBase::TileCoordinates T)
{
    CharacterSprite->moveTo( TileLocation.x * 16 + T.x * 16 + TileOffset.x,
                             TileLocation.y * 8 + T.y * 8 + TileOffset.y);
}

void CharacterBase::Move(TileSystemBase::TileCoordinates T)
{
    CharacterSprite->moveTo((T.x+TileSystem->WorldOffset.x) * 16 + TileOffset.x,(T.y+TileSystem->WorldOffset.y) * 8 + TileOffset.y);
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
bool CharacterBase::isOutOfRange(TileSystemBase::TileCoordinates Target, int Radius)
{
    TileSystemBase::TileCoordinates P = TileSystem->GetWorldCartesian();
    int R = Radius;
    bool bOutOfRange = false;
    for(int i = R; i != -1; i--)
    {
        if((((Target.y <= (P.y + R -i)) && (Target.x <= P.x+i && Target.x >= P.x-i))
            && ((P.y - R + i <= Target.y)))
           || (Target.x <= (P.x + R - i) && (Target.y <= P.x + i && Target.y >= P.x - i)
               && ((Target.x >= P.x - R + i))))
        {

            bOutOfRange = false;
            break;
        }
        else
        {
            bOutOfRange = true;
        }

    }
    return  bOutOfRange;
}

void CharacterBase::SetAnimation(CharacterBase::eAnimation Animation)
{
    this->Animation = Animation;
}





