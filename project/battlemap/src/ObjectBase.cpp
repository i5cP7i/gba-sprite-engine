//
// Created by eduar on 1/25/2022.
//

#include "ObjectBase.h"

ObjectBase::ObjectBase(const void *ImageData, int ImageSize)
{
    ObjectSprite = ObjectBuilder
            .withData(ImageData, ImageSize)
            .withSize(SIZE_16_16)
            .withLocation(x, y)
            .buildPtr();
    ObjectSprite->flipHorizontally(false);
}

void ObjectBase::Move(int x, int y)
{
    ObjectSprite->moveTo(x,y);
}
