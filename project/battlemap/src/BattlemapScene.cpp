//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"

void BattlemapScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(GizaPlainsMapPal, sizeof(GizaPlainsMapPal)));

    EnemyCharacter = std::unique_ptr<Enemy>(new Enemy(100, 102));
    PlayerCharacter = std::unique_ptr<Player>(new Player(80, 102));

    TextStream::instance().setText("FINAL FANTASY TACTICS CLONE!", 3, 1);

    TextStream::instance().setRGB(31,31,31);

    TextStream::instance() << "!!abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!!";

    TextStream::instance().setText("YES!!!", 4, 3);

    TextStream::instance().setRGB(31,0,0);
    TextStream::instance().setText("WERKT!!!", 6, 6);


    #ifdef _DEBUGMODE_0
    playertest = Builder
            .withData(soldierTiles, sizeof(soldierTiles))
            .withSize(SIZE_16_32)
            .withAnimated(24, 30)
            .withLocation(80, 102)
            .buildPtr();

    // playertest->stopAnimating();
    // playertest->setBeginFrame(1);
    #endif
    Battlemap = std::unique_ptr<Background>(new Background(1, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),14,2, BG_REG_64x32));
    bg1 = std::unique_ptr<Background>(new Background(2, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),14,2, BG_REG_64x32));
    bg2 = std::unique_ptr<Background>(new Background(3, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),14,2, BG_REG_64x32));

}

void BattlemapScene::tick(u16 keys)
{
    PlayerCharacter->AnimateWalking();
    EnemyCharacter->AnimateWalking();

    #ifdef _DEBUGMODE_0
    if (keys & KEY_LEFT) // NorthWest
    {
        PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthWest);
    }
    else if (keys & KEY_RIGHT) // SouthEast
    {
        PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthEast);
    }
    else if(keys & KEY_UP) // NorthEast
    {
        PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthEast);
    }
    else if(keys & KEY_DOWN) // SouthWest
    {
        PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthWest);
    }

    else if (keys & KEY_A)
    {
        PlayerCharacter->HandleMovement();
    }

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
    // Battlemap->scroll(offsetX, offsetY);
    #endif
}

std::vector<Sprite *> BattlemapScene::sprites()
{
    return
    {
        #ifdef _DEBUGMODE_0
            playertest.get(),
        #endif
        PlayerCharacter->Get(),
        EnemyCharacter->Get(),
    };

}

std::vector<Background *> BattlemapScene::backgrounds()
{
    return {Battlemap.get(),bg1.get(),bg2.get()};
}
