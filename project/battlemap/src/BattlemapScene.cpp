//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

#include "plains.h"

void BattlemapScene::load()
{
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(plainsPal, sizeof(plainsPal)));

    Battlemap = std::unique_ptr<Background>(new Background(1, plainsTiles, sizeof(plainsTiles), test, sizeof(test)));
    Battlemap.get()->useMapScreenBlock(16);
}

void BattlemapScene::tick(u16 keys)
{
    Battlemap.get()->updateMap(this);
}

std::vector<Sprite *> BattlemapScene::sprites()
{
    return {};
}

std::vector<Background *> BattlemapScene::backgrounds()
{
    return {Battlemap.get()};
}
