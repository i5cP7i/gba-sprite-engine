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
}
