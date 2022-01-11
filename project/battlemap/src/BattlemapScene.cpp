//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"

void BattlemapScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(GizaPlainsMapPal, sizeof(GizaPlainsMapPal)));

    EnemyCharacter = std::unique_ptr<Enemy>(new Enemy(93+4*16, 108-2*8));
    PlayerCharacter = std::unique_ptr<Player>(new Player(78, 101));

    PlayerCharacter->Get()->setPalBank(0);
    EnemyCharacter->Get()->setPalBank(1);

    TileSystem = std::unique_ptr<TileSystemBase>(new TileSystemBase()); // Tile delta_x = 16, delta_y = 8
    TileSystem->Get().at(0)->setPalBank(2);
    TileSystem->Get().at(1)->setPalBank(2);

    TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);

    #ifdef _DEBUGMODE_0
    TileSelector = std::unique_ptr<TileSelection>(new TileSelection()); // Tile delta_x = 16, delta_y = 8
    TileSelector->Get().at(0)->setPalBank(2);
    TileSelector->Get().at(1)->setPalBank(2);

    TextStream::instance().setText("FINAL FANTASY TACTICS CLONE!", 3, 1);

    TextStream::instance().setRGB(31,31,31);

    TextStream::instance() << "!!abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!!";

    TextStream::instance().setText("YES!!!", 4, 3);

    TextStream::instance().setRGB(31,0,0);
    TextStream::instance().setText("WERKT!!!", 6, 6);

    playertest = Builder
            .withData(soldierTiles, sizeof(soldierTiles))
            .withSize(SIZE_16_32)
            .withAnimated(24, 30)
            .withLocation(80, 102)
            .buildPtr();

    // playertest->stopAnimating();
    // playertest->setBeginFrame(1);
    #endif
    Battlemap = std::unique_ptr<Background>(new Background(1, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),4,1, BG_REG_64x32));
    bg1 = std::unique_ptr<Background>(new Background(2, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),4,1, BG_REG_64x32));
    bg2 = std::unique_ptr<Background>(new Background(3, GizaPlainsMapTiles, sizeof(GizaPlainsMapTiles), GizaPlainsMapMap, sizeof(GizaPlainsMapMap),4,1, BG_REG_64x32));

    GameState = eGameState::Setup;
}

void BattlemapScene::tick(u16 keys)
{
    prev_keys = keys;
    keys = REG_KEYINPUT | 0xFC00; // 0XFC00 = KEY_MASK -> Wrongly defined in the Engine!

    if (!isGameOver())
    {
        switch (GameState)
        {
            case eGameState::Setup:
                Setup(keys);
                break;
            case eGameState::Play:
                Play(keys);
                break;
            case eGameState::End:
                End(keys);
                break;
            default:
                break;
        }
    }
    else
    {

    }

    #ifdef _DEBUGMODE_0
    PlayerCharacter->AnimateWalking();
    EnemyCharacter->AnimateWalking();
    TileSystem->Update();

    TileSelector->Update();
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
    std::vector<Sprite *> SpriteCollector;
    SpriteCollector.push_back(PlayerCharacter->Get());
    SpriteCollector.push_back(EnemyCharacter->Get());


    for (int i = 0; i < TileSystem->Get().size(); ++i)
    {
        SpriteCollector.push_back(TileSystem->Get().at(i));
    }

    return
    {
        #ifdef _DEBUGMODE_0
            playertest.get(),
            TileSelector->Get().at(0),
            TileSelector->Get().at(1),
        #endif
            SpriteCollector
    };

}

std::vector<Background *> BattlemapScene::backgrounds()
{
    return {Battlemap.get(),bg1.get(),bg2.get()};
}

bool BattlemapScene::isGameOver()
{
    return GameState == eGameState::End;
}

void BattlemapScene::Setup(u16 keys)
{

    TextStream::instance().setText("FINAL FANTASY TACTICS CLONE", 0, 1);
    TextStream::instance().setText("Press Start to begin!", 2, 4);

    PlayerCharacter->AnimateWalking();
    EnemyCharacter->AnimateWalking();
    if (GetKeyState(keys, KEY_START))
    {
        TextStream::instance().clear();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
        TileSystem->Move(PlayerCharacter->Get()->getX()-8, PlayerCharacter->Get()->getY()+19);
        GameState = eGameState::Play;
        // prev_keys = keys;
        old_key = KEY_START;
    }
}

void BattlemapScene::Play(u16 keys)
{
    PlayerCharacter->AnimateWalking();
    EnemyCharacter->AnimateWalking();
    TileSystem->Update();

    if (isRightKeyRising(keys))
    {
        TileSystem->MoveRight();
    }
    else if (isLeftKeyRising(keys))
    {
        TileSystem->MoveLeft();
    }
    else if (isUpKeyRising(keys))
    {
        TileSystem->MoveUp();
    }
    else if (isDownKeyRising(keys))
    {
        TileSystem->MoveDown();
    }
    TextStream::instance().setText(std::string("TX: " + std::to_string(TileSystem->Get().at(0)->getX())), 0, 0);
    TextStream::instance().setText(std::string("TY: " + std::to_string(TileSystem->Get().at(0)->getY())), 2, 0);
}

void BattlemapScene::End(u16 keys)
{

}

void BattlemapScene::Reset(u16 keys)
{

}

bool BattlemapScene::GetKeyState(u16 keys, u16 key)
{
    return !(key & (keys));
}

bool BattlemapScene::isKeyReleased(u16 keys, u16 key)
{
    return (key) & (keys & ~prev_keys);
}

bool BattlemapScene::isKeyPressed(u16 keys, u16 key)
{
    return (key) & (~keys & prev_keys);
}

bool BattlemapScene::isRightKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_RIGHT) && !RightKeyPressed)
    {
        // TextStream::instance().setText("Right Key Press.", 0, 1);
        RightKeyPressed = true;

        return true;
    }
    else if (isKeyReleased(keys, KEY_RIGHT) && RightKeyPressed)
    {
        // TextStream::instance().setText("Right Key Release.", 0, 1);
        RightKeyPressed = false;

        return false;
    }
    return false;
}

bool BattlemapScene::isLeftKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_LEFT) && !LeftKeyPressed)
    {
        // TextStream::instance().setText("Left Key Press.", 0, 1);
        LeftKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_LEFT) && LeftKeyPressed)
    {
        // TextStream::instance().setText("Left Key Release.", 0, 1);
        LeftKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isUpKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_UP) && !UpKeyPressed)
    {
        // TextStream::instance().setText("Up Key Press.", 0, 1);
        UpKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_UP) && UpKeyPressed)
    {
        // TextStream::instance().setText("Up Key Release.", 0, 1);
        UpKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isDownKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_DOWN) && !DownKeyPressed)
    {
        // TextStream::instance().setText("Down Key Press.", 0, 1);
        DownKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_DOWN) && DownKeyPressed)
    {
        // TextStream::instance().setText("Down Key Release.", 0, 1);
        DownKeyPressed = false;
        return false;
    }
    return false;
}
