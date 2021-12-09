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
#include "soldier.h"

void BattlemapScene::load()
{
    // backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(flying_stuff_bgPal, sizeof(flying_stuff_bgPal)));
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(soldierPal, sizeof(soldierPal)));

    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(soldierTiles, sizeof(soldierTiles))
            .withSize(SIZE_16_32)
            .withAnimated(24, 5)
            .withLocation(10, 10)
            .buildPtr();
    player->stopAnimating();
    player->setBeginFrame(1);
    // Battlemap = std::unique_ptr<Background>(new Background(1, flying_stuff_bgTiles, sizeof(flying_stuff_bgTiles), test, sizeof(test)));
    // Battlemap.get()->useMapScreenBlock(16);
}

void BattlemapScene::tick(u16 keys)
{
    Battlemap.get()->updateMap(this);

    if (player->getCurrentFrame() == 2)
    {
        player->stopAnimating();
    }
    if (!player->isAnimating())
    {
        player->animate();
        player->makeAnimated(1, 2, 10);
    }



}

std::vector<Sprite *> BattlemapScene::sprites()
{
    return {player.get()};
}

std::vector<Background *> BattlemapScene::backgrounds()
{
    return {/*Battlemap.get()*/};
}
