//
// Created by eduar on 1/25/2022.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_OBJECTBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_OBJECTBASE_H

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include "Broadsword.h"

class ObjectBase
{
public:
    ObjectBase(const void *ImageData, int ImageSize);
    void Move(int x,int y);
    void Update() const { ObjectSprite->update(); }
    Sprite* Get() const { return ObjectSprite.get(); }
private:
    int x = 240;
    int y = 180;
protected:
    SpriteBuilder<Sprite> ObjectBuilder;
    std::unique_ptr<Sprite> ObjectSprite;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_OBJECTBASE_H
