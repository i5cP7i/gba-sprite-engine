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

    SpriteBuilder<Sprite> Builder;

    player = Builder
            .withData(soldierTiles, sizeof(soldierTiles))
            .withSize(SIZE_16_32)
            .withAnimated(24, 6)
            .withLocation(10, 10)
            .buildPtr();
    // player->stopAnimating();
    // player->setBeginFrame(1);
    // Battlemap = std::unique_ptr<Background>(new Background(1, flying_stuff_bgTiles, sizeof(flying_stuff_bgTiles), test, sizeof(test)));
    // Battlemap.get()->useMapScreenBlock(16);
}

void BattlemapScene::tick(u16 keys)
{
    static u32 PlayerPrevFrame = 0;
    Battlemap.get()->updateMap(this);

    TextStream::instance().setText(std::to_string(player->getCurrentFrame()), 18, 1);

    if (player->getCurrentFrame() == 2 && PlayerPrevFrame != 2)
    {
        PlayerPrevFrame = player->getCurrentFrame();
        player->animateToFrame(0);
    }
    else if (player->getCurrentFrame() == 1 && PlayerPrevFrame == 2)
    {
        PlayerPrevFrame = player->getCurrentFrame();
        player->animateToFrame(-1);

    }
    player->update();
    if (keys & KEY_LEFT)
    {
        player->flipHorizontally(false);
    }
    else if (keys & KEY_RIGHT)
    {
        player->flipHorizontally(true);
    }
    else if(keys & KEY_UP)
    {
        player->flipHorizontally(true);
    }
    else if(keys & KEY_DOWN)
    {
        player->flipHorizontally(false);
    }
    else if((keys & KEY_A) || (keys & KEY_B))
    {

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
