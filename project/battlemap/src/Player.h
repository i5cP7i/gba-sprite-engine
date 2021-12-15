//
// Created by eduar on 12/14/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_PLAYER_H
#define GBA_SPRITE_ENGINE_PROJECT_PLAYER_H

#include "CharacterBase.h"
#include <libgba-sprite-engine/gba_engine.h>

class Player : public CharacterBase
{
private:
    SpriteBuilder<Sprite> PlayerCharacterBuilder;
    std::unique_ptr<Sprite> PlayerCharacterSprite;
    u32 PlayerPrevFrame = 0;
    u32 PlayerFrameOrientation = 2;
    eDirection PlayerDirection;
    int AnimationDelay;
    int AnimationFrames;
    int x;
    int y;
    int dx;
    int dy;
public:
    void SetDirection(eDirection Direction);
    eDirection GetDirection() const { return Direction; }
    Player(const void *ImageData, int ImageSize, int x, int y, int AnimationDelay, int AnimationFrames);
    void HandleMovement();
    void AnimateWalking();
    void Update() const { PlayerCharacterSprite->update(); }
    Sprite* Get() const { return PlayerCharacterSprite.get(); }
};


#endif //GBA_SPRITE_ENGINE_PROJECT_PLAYER_H
