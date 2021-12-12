//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"


#include "Plains.h"
#include "Soldier.h"

void BattlemapScene::load()
{
    engine.get()->disableText();
    #ifdef _DEBUGMODE_0
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(soldierPal, sizeof(soldierPal)));
    #endif
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(GizaPlainsMapPal, sizeof(GizaPlainsMapPal)));
    Battlemap = std::unique_ptr<Background>(new Background(0, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),16,0, BG_REG_64x32));
    // Battlemap.get()->useMapScreenBlock(8);

    #ifdef _DEBUGMODE_0
    playertest = Builder
            .withData(soldierTiles, sizeof(soldierTiles))
            .withSize(SIZE_16_32)
            .withAnimated(24, 20)
            .withLocation(80, 102)
            .buildPtr();

    // playertest->stopAnimating();
    // playertest->setBeginFrame(1);
    #endif
}

void BattlemapScene::tick(u16 keys)
{
    #ifdef _DEBUGMODE_0
    static u32 PlayerPrevFrame = 0;
    static u32 PlayerFrameOrientation = 2;
    static enum ePlayerDirection {Down, Left, Right, Up} PlayerDirection;

    // static VECTOR PlayerR = {playertest->getX(), playertest->getY()};
    // Battlemap.get()->updateMap(this);

    // TextStream::instance().setText(std::to_string(playertest->getCurrentFrame()), 18, 1);


    if (playertest->getCurrentFrame() == PlayerFrameOrientation && PlayerPrevFrame != PlayerFrameOrientation)
    {
        PlayerPrevFrame = playertest->getCurrentFrame();
        playertest->animateToFrame(PlayerFrameOrientation-2);
    }
    else if (playertest->getCurrentFrame() == PlayerFrameOrientation-1 && PlayerPrevFrame == PlayerFrameOrientation)
    {
        PlayerPrevFrame = playertest->getCurrentFrame();
        playertest->animateToFrame(PlayerFrameOrientation-3);
    }
    playertest->update();

    if (keys & KEY_LEFT)
    {
        playertest->flipHorizontally(false);
        PlayerFrameOrientation = 5;
        PlayerPrevFrame = 3;
        playertest->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Left;
    }
    else if (keys & KEY_RIGHT)
    {
        playertest->flipHorizontally(true);
        PlayerFrameOrientation = 2;
        PlayerPrevFrame = 0;
        playertest->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Right;
    }
    else if(keys & KEY_UP)
    {
        playertest->flipHorizontally(true);
        PlayerFrameOrientation = 5;
        PlayerPrevFrame = 3;
        playertest->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Up;
    }
    else if(keys & KEY_DOWN)
    {
        playertest->flipHorizontally(false);
        PlayerFrameOrientation = 2;
        PlayerPrevFrame = 0;
        playertest->animateToFrame(PlayerFrameOrientation-3);
        PlayerDirection = Down;
    }
    else if((keys & KEY_A))
    {

        switch (PlayerDirection)
        {
            case Down:
                playertest->moveTo(playertest->getX()-2, playertest->getY()+1);
                playertest->update();
                break;
            case Left:
                playertest->moveTo(playertest->getX()-2, playertest->getY()-1);
                playertest->update();
                break;
            case Right:
                playertest->moveTo(playertest->getX()+2, playertest->getY()+1);
                playertest->update();
                break;
            case Up:
                playertest->moveTo(playertest->getX()+2, playertest->getY()-1);
                playertest->update();
                break;
            default:
                break;
        }

    }
    #endif

    // Battlemap->scroll(offsetX, offsetY);
}

std::vector<Sprite *> BattlemapScene::sprites()
{

    return
    {
        #ifdef _DEBUGMODE_0
            playertest.get()
        #endif
    };

}

std::vector<Background *> BattlemapScene::backgrounds()
{
    return {Battlemap.get()};
}
