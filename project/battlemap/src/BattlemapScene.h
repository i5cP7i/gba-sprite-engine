//
// Created by eduar on 12/7/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H

#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include "Player.h"
#include "Enemy.h"
#include "Plains.h"
#include "ForegroundPalette.h"

// #define _DEBUGMODE_0

class BattlemapScene : public Scene
{
private:
    std::unique_ptr<Background> Battlemap;
    // std::unique_ptr<Background> Text;
    std::unique_ptr<Background> bg1;
    std::unique_ptr<Background> bg2;

    std::unique_ptr<Player> PlayerCharacter;
    std::unique_ptr<Enemy> EnemyCharacter;
    #ifdef _DEBUGMODE_0
    SpriteBuilder<Sprite> Builder;
    std::unique_ptr<Sprite> playertest;
    #endif
    int offsetX, offsetY;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    BattlemapScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};
#endif //GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
