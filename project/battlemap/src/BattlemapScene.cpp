//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"

void BattlemapScene::load()
{

    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(GizaPlainsMapPal, sizeof(GizaPlainsMapPal)));

    MenuSystem["main"].SetTable(1, 3);
    MenuSystem["main"]["Move"].SetID(100).Enable(true);
    MenuSystem["main"]["Action"].SetTable(1, 3);
    auto& MenuAction = MenuSystem["main"]["Action"];
    MenuAction["Attack"].SetID(200).Enable(true);
    MenuAction["Items"]["Potion"].SetID(300).Enable(true);
    MenuSystem["main"]["Wait"].SetID(101);
    MenuSystem["end"].SetTable(1,2);
    MenuSystem["end"]["Retry"].SetID(1000).Enable(true);
    MenuSystem["end"]["Quit"].SetID(1001).Enable(true);
    MenuSystem.Build();

    TileSystem = std::unique_ptr<TileSystemBase>(new TileSystemBase()); // Tile delta_x = 16, delta_y = 8
    for (int i = 0; i < TileSystem->Get().size(); ++i)
    {
        TileSystem->Get().at(i)->buildOam(TileSystem->Get().at(i)->getTileIndex(), 3);
    }
    TileSystem->Get().at(0)->setPalBank(2);
    TileSystem->Get().at(1)->setPalBank(2);
    TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);

    EnemyCharacter = std::unique_ptr<Enemy>(new Enemy(TileSystem->GetWorldLocation().x + 13*TileSystem->TileWidth/2, TileSystem->GetWorldLocation().y + 13*TileSystem->TileHeight/2));
    PlayerCharacter = std::unique_ptr<Player>(new Player(TileSystem->GetWorldLocation().x + 3*TileSystem->TileWidth/2, TileSystem->GetWorldLocation().y + 13*TileSystem->TileHeight/2));
    PlayerCharacter->TileSystem->GetWorldTransform(PlayerCharacter->GetTileLocation());
    EnemyCharacter->TileSystem->GetWorldTransform(EnemyCharacter->GetTileLocation());

    PlayerCharacter->Get()->setPalBank(0);
    EnemyCharacter->Get()->setPalBank(1);



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
    Battlemap->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    bg1->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    bg2->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);



    GameState = eGameState::Setup;

    CurrentCharacterSprite = PlayerCharacter->Get();
    OtherCharacterSprite = EnemyCharacter->Get();

    engine->enqueueMusic(test,1872091,16000); // totalSamples = 1952095
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
                if (!bGameOver && isGameStateTransitioning(200) == 0)
                {
                    TextStream::instance().clear();
                    bGameOver = true;
                }
                else
                {
                    End(keys);
                }
                break;
            case eGameState::Reset:
                Reset();
                break;
            default:
                break;
        }
    }
    else
    {
        // engine->setScene(MainMenu);
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


    SpriteCollector.push_back(CurrentCharacterSprite);
    SpriteCollector.push_back(OtherCharacterSprite);


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
    return GameState == eGameState::Quit;
}

void BattlemapScene::Setup(u16 keys)
{

    TextStream::instance().setText("FINAL FANTASY TACTICS CLONE", 0, 1);
    TextStream::instance().setText("Press Start to begin!", 2, 4);
    PlayerCharacter->AnimateHalt();
    EnemyCharacter->AnimateHalt();
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
    PlayerCharacter->TileSystem->UpdateLocation();
    EnemyCharacter->TileSystem->UpdateLocation();
    Menu(keys);

    #ifdef _DEBUGMODE_0
        TextStream::instance().setText(std::string("TX: " + std::to_string(TileSystem->Get().at(0)->getX())), 0, 0);
        TextStream::instance().setText(std::string("TY: " + std::to_string(TileSystem->Get().at(0)->getY())), 1, 0);
        TextStream::instance().setText(std::string("MenuObject: " + std::to_string(MenuSelected)), 2, 0);
    #endif

    Battlemap->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    bg1->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    bg2->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
}

void BattlemapScene::End(u16 keys)
{
    if (OnEndState)
    {
        MenuObject *command = nullptr;
        WinningSprite->animateToFrame(9);
        LosingSprite->animateToFrame(13);

        MenuSystemManager.Open(&MenuSystem["end"]);
        MenuSystemManager.Draw(MenuScreenOffset);
        TextStream::instance().setText("COMPLETED!", 2, 10);

        if (isUpKeyRising(keys))
        {
            MenuSystemManager.OnUp();
        }
        else if (isDownKeyRising(keys))
        {
            MenuSystemManager.OnDown();
        }
        else if (isAKeyRising(keys))
        {
            command = MenuSystemManager.OnConfirm();
        }

        if (command != nullptr)
        {
            switch (command->GetID())
            {
                case 1000:
                    GameState = eGameState::Reset;
                    break;
                case 1001:
                    break;
                default:
                    break;
            }
            MenuSystemManager.OnBack();
        }
    }
    else if (!OnEndState && isGameStateTransitioning(225) == 0)
    {
        MenuScreenOffset += {10,0};
        OnEndState = true;
    }
    else
    {
        WinningSprite->animateToFrame(0);
        LosingSprite->animateToFrame(13);
    }
}

void BattlemapScene::Reset()
{
    TurnID = 0;
    oldTurnID = TurnID;
    bGameOver = false;
    OnEndState = false;


    TileSystem->ResetPos();
    TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);

    PlayerCharacter->Reset();
    EnemyCharacter->Reset();

    MenuSystem["main"]["Move"].Enable(true);
    MenuSystem["main"]["Action"]["Attack"].Enable(true);
    MenuSystem["main"]["Action"]["Items"].Enable(true);
    MenuSystemManager.Open(&MenuSystem["main"]);
    MenuSystemManager.OnResetPosY();
    MenuSystemManager.OnBack();
    MenuSystemManager.OnResetPosY();
    MenuScreenOffset = {1,4};

    TextStream::instance().clear();

    GameMenu = eGameMenu::Init;
    GameState = eGameState::Setup;
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
            ItemMenu(keys);
            break;
        case eGameMenu::Wait:
            WaitMenu(keys);
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
    if (isAKeyRising(keys) || (TurnID != oldTurnID))
    {
        oldTurnID = TurnID;
        if (!MenuSystemManager.isOpen())
        {
            MenuSystemManager.Open(&MenuSystem["main"]);
        }
        else
        {
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
                if (CurrentCharacterSprite == PlayerCharacter->Get())
                {
                    TileSystem->Move(PlayerCharacter->TileSystem->WorldLocation);
                }
                else
                {
                    TileSystem->Move(EnemyCharacter->TileSystem->WorldLocation);
                }

                GameMenu = eGameMenu::Move;
                break;
            case 200:
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                if (CurrentCharacterSprite == PlayerCharacter->Get())
                {
                    TileSystem->Move(PlayerCharacter->TileSystem->WorldLocation);
                }
                else
                {
                    TileSystem->Move(EnemyCharacter->TileSystem->WorldLocation);
                }
                GameMenu = eGameMenu::Attack;
                break;
            case 300:
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                if (CurrentCharacterSprite == PlayerCharacter->Get())
                {
                    TileSystem->Move(PlayerCharacter->TileSystem->WorldLocation);
                }
                else
                {
                    TileSystem->Move(EnemyCharacter->TileSystem->WorldLocation);
                }
                GameMenu = eGameMenu::Items;
                break;
            case 101:
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                if (CurrentCharacterSprite == PlayerCharacter->Get())
                {
                    TileSystem->Move(PlayerCharacter->TileSystem->WorldLocation);
                }
                else
                {
                    TileSystem->Move(EnemyCharacter->TileSystem->WorldLocation);
                }
                GameMenu = eGameMenu::Wait;
                break;
        }
    }
    // TextStream::instance().setText("PX: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().x), 8, 12);
    // TextStream::instance().setText("PY: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().y), 9, 12);
    MenuSystemManager.Draw(MenuScreenOffset);
}

void BattlemapScene::MoveMenu(u16 keys)
{
    PlayerCharacter->TileSystem->UpdateLocation();
    EnemyCharacter->TileSystem->UpdateLocation();

    // TileMoveVector.push_back(PlayerCharacter->GetTileLocation().x +
    // TextStream::instance().setText("PX: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().x), 8, 12);
    // TextStream::instance().setText("PY: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().y), 9, 12);

    if (isRightKeyRising(keys))
    {
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {

        }
        else
        {

        }
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
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        MenuSystemManager.Open(&MenuSystem["main"]);
        MenuSystemManager.OnResetPosY();
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {

        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            CurrentCharacterSpriteDirection = PlayerCharacter->GetDirection();
            OtherCharacterSpriteDirection = EnemyCharacter->GetDirection();
        }
        else
        {
            CurrentCharacterSpriteDirection = EnemyCharacter->GetDirection();
            OtherCharacterSpriteDirection = PlayerCharacter->GetDirection();
        }
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->Move(TileSystem->GetWorldLocation());
        }
        else
        {
            EnemyCharacter->Move(TileSystem->GetWorldLocation());
        }
        if (CurrentCharacterSprite->getX() >= OtherCharacterSprite->getX() && CurrentCharacterSprite->getY() >= OtherCharacterSprite->getY()
            || CurrentCharacterSprite->getX() <= OtherCharacterSprite->getX() && CurrentCharacterSprite->getY() >= OtherCharacterSprite->getY())
        {
            for (int i = 0; i < TileSystem->Get().size(); ++i)
            {
                TileSystem->Get().at(i)->buildOam(TileSystem->Get().at(i)->getTileIndex(), 3);
            }
            CurrentCharacterSprite->buildOam(CurrentCharacterSprite->GetTileIndex(),0);
            OtherCharacterSprite->buildOam(OtherCharacterSprite->GetTileIndex(),1);
        }
        else
        {
            for (int i = 0; i < TileSystem->Get().size(); ++i)
            {
                TileSystem->Get().at(i)->buildOam(TileSystem->Get().at(i)->getTileIndex(), 3);
            }
            CurrentCharacterSprite->buildOam(CurrentCharacterSprite->GetTileIndex(),1);
            OtherCharacterSprite->buildOam(OtherCharacterSprite->GetTileIndex(),0);
        }
        TileSystem->ResetPos();
        MenuSystem["main"]["Move"].Enable(false);
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CurrentCharacterSpriteDirection);
            engine->delay(100);
            EnemyCharacter->SetDirection(OtherCharacterSpriteDirection);
        }
        else if (CurrentCharacterSprite != PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(OtherCharacterSpriteDirection);
            engine->delay(100);
            EnemyCharacter->SetDirection(CurrentCharacterSpriteDirection);
        }
        GameMenu = eGameMenu::Init;
    }
    TextStream::instance().setText("x: " + std::to_string(TileSystem->GetTileLocation().x), 2,1);
    TextStream::instance().setText("y: " + std::to_string(TileSystem->GetTileLocation().y), 3,1);
}

void BattlemapScene::AttackMenu(u16 keys)
{
    PlayerCharacter->TileSystem->UpdateLocation();
    EnemyCharacter->TileSystem->UpdateLocation();

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
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        MenuSystemManager.Open(&MenuSystem["main"]);
        MenuSystemManager.OnResetPosY();
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {
        if (TileSystem->GetTileStatus() != TileSystemBase::eStatus::Invalid
            && ((TileSystem->GetWorldLocation().x == PlayerCharacter->TileSystem->GetWorldLocation().x && TileSystem->GetWorldLocation().y == PlayerCharacter->TileSystem->GetWorldLocation().y)))
        {
            TileSystem->ResetPos();
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
            MenuSystem["main"]["Action"]["Attack"].Enable(false);
            MenuSystem["main"]["Action"]["Items"].Enable(false);

            int Damage = std::rand() % (EnemyCharacter->GetStrength()+EnemyCharacter->GetStrength()/10) + EnemyCharacter->GetStrength()-EnemyCharacter->GetStrength()/10;
            std::string sign;
            if (Damage > 0)
            {
                sign = "-";
            }
            else
            {
                sign = "+";
            }

            PlayerCharacter->SetHealth(PlayerCharacter->GetHealth()-Damage);
            TextStream::instance().setText(sign + std::to_string(Damage), 6,1+9);
            TextStream::instance().setText("Health: " + std::to_string(PlayerCharacter->GetHealth()), 7,1);

        }
        else if (TileSystem->GetTileStatus() != TileSystemBase::eStatus::Invalid
                 && ((TileSystem->GetWorldLocation().x == EnemyCharacter->TileSystem->GetWorldLocation().x && TileSystem->GetWorldLocation().y == EnemyCharacter->TileSystem->GetWorldLocation().y)))
        {
            TileSystem->ResetPos();
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
            MenuSystem["main"]["Action"]["Attack"].Enable(false);
            MenuSystem["main"]["Action"]["Items"].Enable(false);

            int Damage = std::rand() % (PlayerCharacter->GetStrength()+PlayerCharacter->GetStrength()/10) + PlayerCharacter->GetStrength()-PlayerCharacter->GetStrength()/10;
            std::string sign;
            if (Damage > 0)
            {
                sign = "-";
            }
            else
            {
                sign = "+";
            }

            EnemyCharacter->SetHealth(EnemyCharacter->GetHealth()-Damage);
            TextStream::instance().setText(sign + std::to_string(Damage), 6,14+9);
            TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 7,14);


        }
        if (EnemyCharacter->GetHealth() <= 0 || PlayerCharacter->GetHealth() <= 0)
        {
            if (CurrentCharacterSprite == PlayerCharacter->Get() && PlayerCharacter->GetHealth() > 0)
            {
                WinningSprite = CurrentCharacterSprite;
                LosingSprite = OtherCharacterSprite;
            }
            else
            {
                WinningSprite = CurrentCharacterSprite;
                LosingSprite = OtherCharacterSprite;
            }

            GameState = eGameState::End;
        }
        else
        {
            GameMenu = eGameMenu::Init;
        }
    }

    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {
        if (PlayerCharacter->isOutOfRange(TileSystem->GetWorldLocation(), PlayerCharacter->GetAttackRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            if (TileSystem->GetWorldLocation().x != PlayerCharacter->TileSystem->GetWorldLocation().x
                && TileSystem->GetWorldLocation().y != PlayerCharacter->TileSystem->GetWorldLocation().y)
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
            }
            else
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
            }

        }
    }
    else
    {
        if (EnemyCharacter->isOutOfRange(TileSystem->GetWorldLocation(), EnemyCharacter->GetAttackRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            if (TileSystem->GetWorldLocation().x != EnemyCharacter->TileSystem->GetWorldLocation().x
                && TileSystem->GetWorldLocation().y != EnemyCharacter->TileSystem->GetWorldLocation().y)
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
            }
            else
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
            }

        }
    }

    // TextStream::instance().setText("tx: " + std::to_string(TileSystem->GetWorldLocation().x), 2,1);
    // TextStream::instance().setText("ty: " + std::to_string(TileSystem->GetWorldLocation().y), 3,1);
    // TextStream::instance().setText("px: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().x), 4,1);
    // TextStream::instance().setText("py: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().y), 5,1);
    // TextStream::instance().setText("ex: " + std::to_string(EnemyCharacter->TileSystem->GetWorldLocation().x), 5,1);
    // TextStream::instance().setText("ey: " + std::to_string(EnemyCharacter->TileSystem->GetWorldLocation().y), 6,1);
}

void BattlemapScene::ItemMenu(u16 keys)
{
    PlayerCharacter->TileSystem->UpdateLocation();
    EnemyCharacter->TileSystem->UpdateLocation();

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
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        MenuSystemManager.Open(&MenuSystem["main"]);
        MenuSystemManager.OnResetPosY();
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {
        if (TileSystem->GetTileStatus() != TileSystemBase::eStatus::Invalid
            && ((TileSystem->GetWorldLocation().x == PlayerCharacter->TileSystem->GetWorldLocation().x && TileSystem->GetWorldLocation().y == PlayerCharacter->TileSystem->GetWorldLocation().y)))
        {
            TileSystem->ResetPos();
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
            MenuSystem["main"]["Action"]["Attack"].Enable(false);
            MenuSystem["main"]["Action"]["Items"].Enable(false);

            PlayerCharacter->SetHealth(PlayerCharacter->GetHealth()+30);
            TextStream::instance().setText("+30!", 6,1+9);
            TextStream::instance().setText("Health: " + std::to_string(PlayerCharacter->GetHealth()), 7,1);

            GameMenu = eGameMenu::Init;
        }
        else if (TileSystem->GetTileStatus() != TileSystemBase::eStatus::Invalid
                 && ((TileSystem->GetWorldLocation().x == EnemyCharacter->TileSystem->GetWorldLocation().x && TileSystem->GetWorldLocation().y == EnemyCharacter->TileSystem->GetWorldLocation().y)))
        {
            TileSystem->ResetPos();
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
            MenuSystem["main"]["Action"]["Attack"].Enable(false);
            MenuSystem["main"]["Action"]["Items"].Enable(false);

            EnemyCharacter->SetHealth(EnemyCharacter->GetHealth()+30);
            TextStream::instance().setText("+30!", 6,14+9);
            TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 7,14);

            GameMenu = eGameMenu::Init;
        }
    }
    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {
        if (PlayerCharacter->isOutOfRange(TileSystem->GetWorldLocation(), PlayerCharacter->GetAttackRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
        }
    }
    else
    {
        if (EnemyCharacter->isOutOfRange(TileSystem->GetWorldLocation(), EnemyCharacter->GetAttackRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
        }
    }
}


void BattlemapScene::WaitMenu(u16 keys)
{
    PlayerCharacter->TileSystem->UpdateLocation();
    EnemyCharacter->TileSystem->UpdateLocation();

    if (isRightKeyRising(keys))
    {
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthEast);
        }
        else
        {
            EnemyCharacter->SetDirection(CharacterBase::eDirection::SouthEast);
        }
    }
    else if (isLeftKeyRising(keys))
    {
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthWest);
        }
        else
        {
            EnemyCharacter->SetDirection(CharacterBase::eDirection::NorthWest);
        }
    }
    else if (isUpKeyRising(keys))
    {
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthEast);
        }
        else
        {
            EnemyCharacter->SetDirection(CharacterBase::eDirection::NorthEast);
        }
    }
    else if (isDownKeyRising(keys))
    {
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthWest);
        }
        else
        {
            EnemyCharacter->SetDirection(CharacterBase::eDirection::SouthWest);
        }
    }
    else if (isBKeyRising(keys))
    {
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        MenuSystemManager.Open(&MenuSystem["main"]);
        MenuSystemManager.OnResetPosY();
        GameMenu = eGameMenu::Init;
    }
    else if (isAKeyRising(keys))
    {
        TurnID++;
        if (TurnID % 2 == 0)
        {
            CurrentCharacterSprite = PlayerCharacter->Get();
            OtherCharacterSprite = EnemyCharacter->Get();
        }
        else
        {
            CurrentCharacterSprite = EnemyCharacter->Get();
            OtherCharacterSprite = PlayerCharacter->Get();
        }
        TileSystem->ResetPos();
        TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
        MenuSystem["main"]["Move"].Enable(true);
        MenuSystem["main"]["Action"]["Attack"].Enable(true);
        MenuSystem["main"]["Action"]["Items"].Enable(true);
        MenuSystemManager.Open(&MenuSystem["main"]);
        MenuSystemManager.OnResetPosY();
        GameMenu = eGameMenu::Init;
    }
}

unsigned char BattlemapScene::ClipValue(unsigned char Number, unsigned char LowerBound, unsigned char UpperBound)
{
    return std::max(LowerBound, std::min(Number, UpperBound));
}

// TODO: Source
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

bool BattlemapScene::isGameStateTransitioning(unsigned Delay)
{
    static unsigned TransitionDelay = 0;
    TransitionDelay++;
    if (TransitionDelay >= Delay)
    {
        TransitionDelay = 0;
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

void BattlemapScene::ProcessBackgroundScrolling(TileSystemBase::TileCoordinates TilePosition)
{
    // for (int i = 0; i < )
}



