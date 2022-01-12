//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"

void BattlemapScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(GizaPlainsMapPal, sizeof(GizaPlainsMapPal)));

    /*
    MenuSystem["main"].SetTable(1, 10);
    MenuSystem["main"]["Move"].SetID(101);
    MenuSystem["main"]["Action"]["Attack"].SetID(102).Enable(true);
    MenuSystem["main"]["Action"]["Items"]["Potion"].SetID(102).Enable(true);
    MenuSystem["main"]["Wait"].SetID(103);
    */
    MenuSystem["main"].SetTable(1, 3);
    MenuSystem["main"]["Move"].SetID(100);
    MenuSystem["main"]["Action"].SetTable(1, 3);
    auto& MenuAction = MenuSystem["main"]["Action"];
    MenuAction["Attack"].SetID(200);
    MenuAction["Item"]["Potion"].SetID(300);

    MenuSystem["main"]["Wait"].SetID(101);
    MenuSystem.Build();


    EnemyCharacter = std::unique_ptr<Enemy>(new Enemy(96+4*16, 124-2*8));
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
        // TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
        // TileSystem->Move(PlayerCharacter->Get()->getX()-8, PlayerCharacter->Get()->getY()+19);
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
    Menu(keys);

    #ifdef _DEBUGMODE_0
        TextStream::instance().setText(std::string("TX: " + std::to_string(TileSystem->Get().at(0)->getX())), 0, 0);
        TextStream::instance().setText(std::string("TY: " + std::to_string(TileSystem->Get().at(0)->getY())), 1, 0);
        TextStream::instance().setText(std::string("MenuObject: " + std::to_string(MenuSelected)), 2, 0);
    #endif

    Battlemap->scroll(bglerpX, bglerpY);
    bg1->scroll(bglerpX, bglerpY);
    bg2->scroll(bglerpX, bglerpY);
}

void BattlemapScene::End(u16 keys)
{

}

void BattlemapScene::Reset(u16 keys)
{

}

void BattlemapScene::Menu(u16 keys)
{
    switch (GameMenu)
    {
        case eGameMenu::Init:
        {
            InitMenu(keys);
            break;
        }
        case eGameMenu::Move:
            MoveMenu(keys);
            break;
        case eGameMenu::Attack:
            AttackMenu(keys);
            break;
        case eGameMenu::Items:
            break;
        case eGameMenu::Wait:
            break;
        default:
            break;
    }


    #ifdef _DEBUGMODE_0
    if (MenuSelected >= 1 && isDownKeyRising(keys))
    {
        MenuSelected++;
        MenuSelected = ClipValue(MenuSelected, 1, 3);
        MenuSelect = static_cast<eMenuSelect>(MenuSelected);
        TextStream::instance().clear();
    }
    else if (MenuSelected >= 1 && isUpKeyRising(keys))
    {
        MenuSelected--;
        MenuSelected = ClipValue(MenuSelected, 1, 3);
        MenuSelect = static_cast<eMenuSelect>(MenuSelected);
        TextStream::instance().clear();
    }
    else if (isAKeyRising(keys))
    {
        switch (MenuSelect)
        {
            case eMenuSelect::Init:
                MenuSelected = 1;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Init;
                break;
            case eMenuSelect::Move:
                MenuSelected = 0;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Move;
                break;
            case eMenuSelect::Action:

                break;
            case eMenuSelect::Wait:

                break;
            default:
                break;
        }
    }
    else if (isBKeyRising(keys))
    {
        switch (GameMenu)
        {
            case eGameMenu::Init:
                MenuSelected = 0;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Init;
                break;
            case eGameMenu::Move:
                MenuSelected = 1;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Init;
                break;
            case eGameMenu::Action:
                MenuSelected = 1;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Init;
                break;
            case eGameMenu::Wait:
                MenuSelected = 1;
                MenuSelect = static_cast<eMenuSelect>(MenuSelected);
                GameMenu = eGameMenu::Init;
                break;
            default:
                break;
        }
    }
    switch(MenuSelect)
    {
        case eMenuSelect::Init:
            TextStream::instance().clear();
            break;
        case eMenuSelect::Move:
            TextStream::instance().setText("MenuObject", 6, 4);
            TextStream::instance().setText("->Move", 7, 1);
            TextStream::instance().setText("Action", 8, 3);
            TextStream::instance().setText("Wait", 9, 3);
            break;
        case eMenuSelect::Action:
            TextStream::instance().setText("MenuObject", 6, 4);
            TextStream::instance().setText("Move", 7, 3);
            TextStream::instance().setText("->Action", 8, 1);
            TextStream::instance().setText("Wait", 9, 3);
            break;
        case eMenuSelect::Wait:
            TextStream::instance().setText("MenuObject", 6, 4);
            TextStream::instance().setText("Move", 7, 3);
            TextStream::instance().setText("Action", 8, 3);
            TextStream::instance().setText("->Wait", 9, 1);
            break;
        default:
            break;
    }

    switch (GameMenu)
    {
        case eGameMenu::Init:
            break;
        case eGameMenu::Move:
            MoveMenu(keys);
            break;
        case eGameMenu::Action:
            break;
        case eGameMenu::Items:
            break;
        case eGameMenu::Wait:
            break;
        default:
            break;
    }
    #endif
}

void BattlemapScene::InitMenu(u16 keys)
{
    // MenuSystem["main"].DrawSelf(MenuScreenOffset);
    MenuObject *command = nullptr;

    if (isAKeyRising(keys)) {
        if (!MenuSystemManager.isOpen()) {
            MenuSystemManager.Open(&MenuSystem["main"]);
        } else {
            command = MenuSystemManager.OnConfirm();
        }
    }
    if (isDownKeyRising(keys)) MenuSystemManager.OnDown();
    if (isUpKeyRising(keys)) MenuSystemManager.OnUp();
    if (isBKeyRising(keys)) {
        MenuSystemManager.OnBack();
    }
    TextStream::instance().setText("M.bO.: " + std::to_string(MenuSystemManager.isOpen()), 0, 1);
    if (command != nullptr)
    {
        MenuSystem.sLastAction = "L.Act.: " + command->GetName() + "ID: " + std::to_string(command->GetID());
        MenuSystemManager.Close();
        TextStream::instance().setText(MenuSystem.sLastAction, 1, 1);
        switch (command->GetID())
        {
            case 100:
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                TileSystem->Move(PlayerCharacter->Get()->getX() - 8, PlayerCharacter->Get()->getY() + 19);
                GameMenu = eGameMenu::Move;
                break;
            case 200:
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                TileSystem->Move(PlayerCharacter->Get()->getX() - 8, PlayerCharacter->Get()->getY() + 19);
                GameMenu = eGameMenu::Attack;
        }
    }

    MenuSystemManager.Draw(MenuScreenOffset);
}

void BattlemapScene::MoveMenu(u16 keys)
{
    if (isRightKeyRising(keys))
    {
        TileSystem->MoveRight(offsetX, offsetY);
    }
    else if (isLeftKeyRising(keys))
    {
        TileSystem->MoveLeft(offsetX, offsetY);
    }
    else if (isUpKeyRising(keys))
    {
        TileSystem->MoveUp(offsetX, offsetY);
    }
    else if (isDownKeyRising(keys))
    {
        TileSystem->MoveDown(offsetX, offsetY);
    }
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {
        PlayerCharacter->Get()->moveTo(TileSystem->x+8, TileSystem->y-19);
    }
}

void BattlemapScene::AttackMenu(u16 keys)
{
    if (isRightKeyRising(keys))
    {
        TileSystem->MoveRight(offsetX, offsetY);
    }
    else if (isLeftKeyRising(keys))
    {
        TileSystem->MoveLeft(offsetX, offsetY);
    }
    else if (isUpKeyRising(keys))
    {
        TileSystem->MoveUp(offsetX, offsetY);
    }
    else if (isDownKeyRising(keys))
    {
        TileSystem->MoveDown(offsetX, offsetY);
    }
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {

    }
}

unsigned char BattlemapScene::ClipValue(unsigned char Number, unsigned char LowerBound, unsigned char UpperBound)
{
    return std::max(LowerBound, std::min(Number, UpperBound));
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

bool BattlemapScene::isStartKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_START) && !StartKeyPressed)
    {
        // TextStream::instance().setText("Start Key Press.", 4, 1);
        StartKeyPressed = true;

        return true;
    }
    else if (isKeyReleased(keys, KEY_START) && StartKeyPressed)
    {
        // TextStream::instance().setText("Start Key Release.", 4, 1);
        StartKeyPressed = false;

        return false;
    }
    return false;
}

bool BattlemapScene::isRightKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_RIGHT) && !RightKeyPressed)
    {
        // TextStream::instance().setText("Right Key Press.", 4, 1);
        RightKeyPressed = true;

        return true;
    }
    else if (isKeyReleased(keys, KEY_RIGHT) && RightKeyPressed)
    {
        // TextStream::instance().setText("Right Key Release.", 4, 1);
        RightKeyPressed = false;

        return false;
    }
    return false;
}

bool BattlemapScene::isLeftKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_LEFT) && !LeftKeyPressed)
    {
        // TextStream::instance().setText("Left Key Press.", 4, 1);
        LeftKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_LEFT) && LeftKeyPressed)
    {
        // TextStream::instance().setText("Left Key Release.", 4, 1);
        LeftKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isUpKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_UP) && !UpKeyPressed)
    {
        // TextStream::instance().setText("Up Key Press.", 4, 1);
        UpKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_UP) && UpKeyPressed)
    {
        // TextStream::instance().setText("Up Key Release.", 4, 1);
        UpKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isDownKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_DOWN) && !DownKeyPressed)
    {
        // TextStream::instance().setText("Down Key Press.", 4, 1);
        DownKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_DOWN) && DownKeyPressed)
    {
        // TextStream::instance().setText("Down Key Release.", 4, 1);
        DownKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isAKeyRising(u16 keys) {
    if (isKeyPressed(keys, KEY_A) && !AKeyPressed)
    {
        // TextStream::instance().setText("A Key Press.", 4, 1);
        AKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_A) && AKeyPressed)
    {
        // TextStream::instance().setText("A Key Release.", 4, 1);
        AKeyPressed = false;
        return false;
    }
    return false;
}

bool BattlemapScene::isBKeyRising(u16 keys)
{
    if (isKeyPressed(keys, KEY_B) && !BKeyPressed)
    {
        // TextStream::instance().setText("B Key Press.", 4, 1);
        BKeyPressed = true;
        return true;
    }
    else if (isKeyReleased(keys, KEY_B) && BKeyPressed)
    {
        // TextStream::instance().setText("B Key Release.", 4, 1);
        BKeyPressed = false;
        return false;
    }
    return false;
}



