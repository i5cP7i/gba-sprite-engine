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
    static u32 PlayerFrameOrientation = 2;
    static enum ePlayerDirection {Down, Left, Right, Up} PlayerDirection;

    static VECTOR PlayerR = {player->getX(), player->getY()};
    Battlemap.get()->updateMap(this);

    // TextStream::instance().setText(std::to_string(player->getCurrentFrame()), 18, 1);

    if (player->getCurrentFrame() == PlayerFrameOrientation && PlayerPrevFrame != PlayerFrameOrientation)
    {
        PlayerPrevFrame = player->getCurrentFrame();
        player->animateToFrame(PlayerFrameOrientation-2);
    }
    else if (player->getCurrentFrame() == PlayerFrameOrientation-1 && PlayerPrevFrame == PlayerFrameOrientation)
    {
        PlayerPrevFrame = player->getCurrentFrame();
        player->animateToFrame(PlayerFrameOrientation-3);
    }
    player->update();

    if (keys & KEY_LEFT)
    {
        player->flipHorizontally(false);
        PlayerFrameOrientation = 5;
        PlayerPrevFrame = 3;
        player->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Left;
    }
    else if (keys & KEY_RIGHT)
    {
        player->flipHorizontally(true);
        PlayerFrameOrientation = 2;
        PlayerPrevFrame = 0;
        player->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Right;
    }
    else if(keys & KEY_UP)
    {
        player->flipHorizontally(true);
        PlayerFrameOrientation = 5;
        PlayerPrevFrame = 3;
        player->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Up;
    }
    else if(keys & KEY_DOWN)
    {
        player->flipHorizontally(false);
        PlayerFrameOrientation = 2;
        PlayerPrevFrame = 0;
        player->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Down;
    }
    else if((keys & KEY_A))
    {
        switch (PlayerDirection)
        {
            case Down:
                player->moveTo(player->getX()-1, player->getY()+1);
                player->update();
                break;
            case Left:
                player->moveTo(player->getX()-1, player->getY()-1);
                player->update();
                break;
            case Right:
                player->moveTo(player->getX()+1, player->getY()+1);
                player->update();
                break;
            case Up:
                player->moveTo(player->getX()+1, player->getY()-1);
                player->update();
                break;
            default:
                break;
        }
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
