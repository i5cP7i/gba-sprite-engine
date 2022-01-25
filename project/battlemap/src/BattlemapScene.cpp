//
// Created by eduar on 12/7/2021.
//

#include "BattlemapScene.h"
#include "MainMenuScene.h"

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

    EnemyCharacter = std::unique_ptr<Enemy>(new Enemy("Red", TileSystem->GetWorldLocation().x + 13*TileSystem->TileWidth/2, TileSystem->GetWorldLocation().y + 13*TileSystem->TileHeight/2));
    PlayerCharacter = std::unique_ptr<Player>(new Player("Blue", TileSystem->GetWorldLocation().x + 3*TileSystem->TileWidth/2, TileSystem->GetWorldLocation().y + 13*TileSystem->TileHeight/2));
    // PlayerCharacter->TileSystem->GetWorldTransform(PlayerCharacter->GetTileLocation());
    // EnemyCharacter->TileSystem->GetWorldTransform(EnemyCharacter->GetTileLocation());

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

    ScrollBackground(0,0);
    // Battlemap->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    // bg1->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    // bg2->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);

    GameState = eGameState::Setup;

    CurrentCharacterSprite = PlayerCharacter->Get();
    OtherCharacterSprite = EnemyCharacter->Get();

    // engine->enqueueMusic(test,1872091,16000); // totalSamples = 1952095
}

void BattlemapScene::tick(u16 keys)
{
    BackgroundScroll = TileSystem->GetWorldCartesian();
    // ProcessBackgroundScrolling();

    PlayerCharacter->Update();
    EnemyCharacter->Update();
    prev_keys = keys;
    keys = REG_KEYINPUT | 0xFC00; // 0XFC00 = KEY_MASK

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
        if (!engine->isTransitioning())
        {
            // engine->enqueueSound(zelda_secret_16K_mono, zelda_secret_16K_mono_bytes);
            engine->transitionIntoScene(new MainMenuScene(engine), new FadeOutScene(2));
        }
    }
}

std::vector<Sprite *> BattlemapScene::sprites()
{
    std::vector<Sprite *> SpriteCollector;


    SpriteCollector.push_back(CurrentCharacterSprite);
    SpriteCollector.push_back(OtherCharacterSprite);
    SpriteCollector.push_back(PlayerCharacter->GetWeaponSprite());
    SpriteCollector.push_back(EnemyCharacter->GetWeaponSprite());


    for (int i = 0; i < TileSystem->Get().size(); ++i)
    {
        SpriteCollector.push_back(TileSystem->Get().at(i));
    }

    return
    {
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
    // Battlemap->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    // bg1->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);
    // bg2->scroll(bglerpX+TileSystem->WorldOrigin.x, bglerpY+TileSystem->WorldOrigin.y);

    TextStream::instance().setText("Press Start to begin!", 2, 4);
    PlayerCharacter->AnimateHalt();
    EnemyCharacter->AnimateHalt();
    TileSystem->ResetPos();


    if (GetKeyState(keys, KEY_START))
    {
        TextStream::instance().clear();
        // TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
        // TileSystem->Move(PlayerCharacter->Get()->getX()-8, PlayerCharacter->Get()->getY()+19);
        GameState = eGameState::Play;
        PlayerCharacter->TileSystem->UpdateLocation();
        EnemyCharacter->TileSystem->UpdateLocation();
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            TileSystem->Move(PlayerCharacter->TileSystem->WorldLocation);
        }
        else
        {
            TileSystem->Move(EnemyCharacter->TileSystem->WorldLocation);
        }
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
                    GameState = eGameState::Quit;
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
    CurrentCharacterSpriteDirection = CharacterBase::eDirection::SouthWest;
    OtherCharacterSpriteDirection = CharacterBase::eDirection::SouthWest;

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
}

void BattlemapScene::InitMenu(u16 keys)
{
    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {
        TextStream::instance().setText(PlayerCharacter->GetName(), 0,1);
        TextStream::instance().setText("Health: " + std::to_string(PlayerCharacter->GetHealth()), 1,1);
    }
    else
    {
        TextStream::instance().setText(EnemyCharacter->GetName(), 0,14);
        TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 1,14);
    }
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
    // TextStream::instance().setText("M.bO.: " + std::to_string(MenuSystemManager.isOpen()), 0, 1);
    if (command != nullptr)
    {
        // MenuSystem.sLastAction = "L.Act.: " + command->GetName() + "ID: " + std::to_string(command->GetID());
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
                PreviousTileLocation = TileSystem->GetWorldCartesian();

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
                if (CurrentCharacterSprite == PlayerCharacter->Get())
                {
                    CurrentCharacterSpriteDirection = PlayerCharacter->GetDirection();
                }
                else
                {
                    CurrentCharacterSpriteDirection = EnemyCharacter->GetDirection();
                }
                GameMenu = eGameMenu::Wait;
                break;
        }
    }
    // TextStream::instance().setText("PX: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().x), 8, 12);
    // =TextStream::instance().setText("PY: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().y), 9, 12);
    MenuSystemManager.Draw(MenuScreenOffset);
}

void BattlemapScene::MoveMenu(u16 keys)
{
    CurrentTileLocation = TileSystem->GetWorldCartesian();
    // TileSystem->UpdateLocation();
    // PlayerCharacter->TileSystem->UpdateLocation();
    // EnemyCharacter->TileSystem->UpdateLocation();

    // TileMoveVector.push_back(PlayerCharacter->GetTileLocation().x +
    // TextStream::instance().setText("PX: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().x), 8, 12);
    // TextStream::instance().setText("PY: " + std::to_string(PlayerCharacter->TileSystem->GetWorldLocation().y), 9, 12);
    // TextStream::instance().setText("TX: " + std::to_string(TileSystem->GetWorldCartesian().x), 3, 12);
    // TextStream::instance().setText("TY: " + std::to_string(TileSystem->GetWorldCartesian().y), 4, 12);
    // TextStream::instance().setText("PTX: " + std::to_string(PreviousTileLocation.x), 7, 12);
    // TextStream::instance().setText("PTY: " + std::to_string(PreviousTileLocation.y), 8, 12);
    // TextStream::instance().setText("TX: " + std::to_string(CurrentTileLocation.x), 5, 12);
    // TextStream::instance().setText("TY: " + std::to_string(CurrentTileLocation.y), 6, 12);
    // TextStream::instance().setText("WOX: " + std::to_string(TileSystem->WorldOffset.x), 9, 12);
    // TextStream::instance().setText("WOY: " + std::to_string(TileSystem->WorldOffset.y), 10, 12);


    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {
        TextStream::instance().setText(PlayerCharacter->GetName(), 0,1);
        TextStream::instance().setText("Health: " + std::to_string(PlayerCharacter->GetHealth()), 1,1);
    }
    else
    {
        TextStream::instance().setText(EnemyCharacter->GetName(), 0,14);
        TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 1,14);
    }


    if (isRightKeyRising(keys))
    {
        PreviousTileLocation = CurrentTileLocation;
        TileSystem->MoveRight();

    }
    else if (isLeftKeyRising(keys))
    {
        PreviousTileLocation = CurrentTileLocation;
        TileSystem->MoveLeft();

    }
    else if (isUpKeyRising(keys))
    {
        PreviousTileLocation = CurrentTileLocation;
        TileSystem->MoveUp();
    }
    else if (isDownKeyRising(keys))
    {
        PreviousTileLocation = CurrentTileLocation;
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
        if (TileSystem->GetTileStatus() == TileSystemBase::eStatus::Valid)
        {
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
                EnemyCharacter->SetDirection(OtherCharacterSpriteDirection);
            }
            else
            {
                PlayerCharacter->SetDirection(OtherCharacterSpriteDirection);
                EnemyCharacter->SetDirection(CurrentCharacterSpriteDirection);
            }
            GameMenu = eGameMenu::Init;
        }
    }

    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {

        if (PlayerCharacter->isOutOfRange(this->TileSystem->GetWorldCartesian(), PlayerCharacter->GetMoveRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            if (!(TileSystem->GetWorldCartesian().x == PlayerCharacter->TileSystem->GetWorldCartesian().x
                  && TileSystem->GetWorldCartesian().y == PlayerCharacter->TileSystem->GetWorldCartesian().y)
                  && !(TileSystem->GetWorldCartesian().x == EnemyCharacter->TileSystem->GetWorldCartesian().x
                       && TileSystem->GetWorldCartesian().y == EnemyCharacter->TileSystem->GetWorldCartesian().y))
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
        if (EnemyCharacter->isOutOfRange(TileSystem->GetWorldCartesian(), EnemyCharacter->GetMoveRadius()))
        {
            TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
        }
        else
        {
            if (!(TileSystem->GetWorldCartesian().x == EnemyCharacter->TileSystem->GetWorldCartesian().x
                  && TileSystem->GetWorldCartesian().y == EnemyCharacter->TileSystem->GetWorldCartesian().y)
                  && !(TileSystem->GetWorldCartesian().x == PlayerCharacter->TileSystem->GetWorldCartesian().x
                       && TileSystem->GetWorldCartesian().y == PlayerCharacter->TileSystem->GetWorldCartesian().y))
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
            }
            else
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
            }

        }
    }
}

void BattlemapScene::AttackMenu(u16 keys)
{
    // PlayerCharacter->TileSystem->UpdateLocation();
    // EnemyCharacter->TileSystem->UpdateLocation();
    if (MenuSystem["main"]["Action"]["Items"].isEnabled())
    {
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
                EnemyCharacter->SetAnimation(CharacterBase::eAnimation::Attacking);
                PlayerCharacter->GetWeaponSprite()->buildOam(PlayerCharacter->GetWeaponSprite()->GetTileIndex(), 3);
                EnemyCharacter->GetWeaponSprite()->buildOam(EnemyCharacter->GetWeaponSprite()->GetTileIndex(), 3);
                // TODO: Attack Direction
                if (EnemyCharacter->TileSystem->GetWorldCartesian().x == PlayerCharacter->TileSystem->GetWorldCartesian().x)
                {
                    if (EnemyCharacter->TileSystem->GetWorldCartesian().y > PlayerCharacter->TileSystem->GetWorldCartesian().y)
                    {
                        EnemyCharacter->SetDirection(CharacterBase::eDirection::NorthEast);
                    }
                    else
                    {
                        EnemyCharacter->SetDirection(CharacterBase::eDirection::SouthWest);
                    }

                }
                else
                {
                    if (EnemyCharacter->TileSystem->GetWorldCartesian().x > PlayerCharacter->TileSystem->GetWorldCartesian().x)
                    {
                        EnemyCharacter->SetDirection(CharacterBase::eDirection::NorthWest);
                    }
                    else
                    {
                        EnemyCharacter->SetDirection(CharacterBase::eDirection::SouthEast);
                    }
                }

            }
            else if (TileSystem->GetTileStatus() != TileSystemBase::eStatus::Invalid
                     && ((TileSystem->GetWorldLocation().x == EnemyCharacter->TileSystem->GetWorldLocation().x && TileSystem->GetWorldLocation().y == EnemyCharacter->TileSystem->GetWorldLocation().y)))
            {
                TileSystem->ResetPos();
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Inactive);
                MenuSystem["main"]["Action"]["Attack"].Enable(false);
                MenuSystem["main"]["Action"]["Items"].Enable(false);
                PlayerCharacter->SetAnimation(CharacterBase::eAnimation::Attacking);
                PlayerCharacter->GetWeaponSprite()->buildOam(PlayerCharacter->GetWeaponSprite()->GetTileIndex(), 3);
                EnemyCharacter->GetWeaponSprite()->buildOam(EnemyCharacter->GetWeaponSprite()->GetTileIndex(), 3);
                // TODO: Attack Direction
                if (PlayerCharacter->TileSystem->GetWorldCartesian().x == EnemyCharacter->TileSystem->GetWorldCartesian().x)
                {
                    if (PlayerCharacter->TileSystem->GetWorldCartesian().y > EnemyCharacter->TileSystem->GetWorldCartesian().y)
                    {
                        PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthEast);
                    }
                    else
                    {
                        PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthWest);
                    }

                }
                else
                {
                    if (PlayerCharacter->TileSystem->GetWorldCartesian().x > EnemyCharacter->TileSystem->GetWorldCartesian().x)
                    {
                        PlayerCharacter->SetDirection(CharacterBase::eDirection::NorthWest);
                    }
                    else
                    {
                        PlayerCharacter->SetDirection(CharacterBase::eDirection::SouthEast);
                    }
                }
            }
        }

        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {

            if (PlayerCharacter->isOutOfRange(this->TileSystem->GetWorldCartesian(), PlayerCharacter->GetAttackRadius()))
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
            }
            else
            {
                if (!(TileSystem->GetWorldCartesian().x == PlayerCharacter->TileSystem->GetWorldCartesian().x
                      && TileSystem->GetWorldCartesian().y == PlayerCharacter->TileSystem->GetWorldCartesian().y))
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
            if (EnemyCharacter->isOutOfRange(TileSystem->GetWorldCartesian(), EnemyCharacter->GetAttackRadius()))
            {
                TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
            }
            else
            {
                if (!(TileSystem->GetWorldCartesian().x == EnemyCharacter->TileSystem->GetWorldCartesian().x
                      && TileSystem->GetWorldCartesian().y == EnemyCharacter->TileSystem->GetWorldCartesian().y))
                {
                    TileSystem->SetTileStatus(TileSystemBase::eStatus::Valid);
                }
                else
                {
                    TileSystem->SetTileStatus(TileSystemBase::eStatus::Invalid);
                }

            }
        }
    }
    else
    {
        // TODO: check AttackAnimation ended bool. If ended, add damage code and game end condition
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            if (PlayerCharacter->AnimateAttack())
            {
                PlayerCharacter->SetAnimation(CharacterBase::eAnimation::Walking);
                PlayerCharacter->SetDirection(PlayerCharacter->GetDirection());
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
                TextStream::instance().setText(EnemyCharacter->GetName(), 5,14);
                TextStream::instance().setText(sign + std::to_string(Damage), 6,14+9);
                TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 7,14);
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
        }
        else
        {
            if (EnemyCharacter->AnimateAttack())
            {
                EnemyCharacter->SetAnimation(CharacterBase::eAnimation::Walking);
                EnemyCharacter->SetDirection(EnemyCharacter->GetDirection());
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
                TextStream::instance().setText(PlayerCharacter->GetName(), 5,1);
                TextStream::instance().setText(sign + std::to_string(Damage), 6,1+9);
                TextStream::instance().setText("Health: " + std::to_string(PlayerCharacter->GetHealth()), 7,1);
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
        }
    }

    // TextStream::instance().setText("px: " + std::to_string(PlayerCharacter->TileSystem->GetWorldCartesian().x), 4,1);
    // TextStream::instance().setText("py: " + std::to_string(PlayerCharacter->TileSystem->GetWorldCartesian().y), 5,1);
    // TextStream::instance().setText("ex: " + std::to_string(EnemyCharacter->TileSystem->GetWorldCartesian().x), 6,1);
    // TextStream::instance().setText("ey: " + std::to_string(EnemyCharacter->TileSystem->GetWorldCartesian().y), 7,1);
}

void BattlemapScene::ItemMenu(u16 keys)
{
    // PlayerCharacter->TileSystem->UpdateLocation();
    // EnemyCharacter->TileSystem->UpdateLocation();

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
            TextStream::instance().setText(PlayerCharacter->GetName(), 5,1);
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
            TextStream::instance().setText(EnemyCharacter->GetName(), 5,14);
            TextStream::instance().setText("+30!", 6,14+9);
            TextStream::instance().setText("Health: " + std::to_string(EnemyCharacter->GetHealth()), 7,14);

            GameMenu = eGameMenu::Init;
        }
    }
    if (CurrentCharacterSprite == PlayerCharacter->Get())
    {
        if (PlayerCharacter->isOutOfRange(TileSystem->GetWorldCartesian(), PlayerCharacter->GetAttackRadius()))
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
        if (EnemyCharacter->isOutOfRange(TileSystem->GetWorldCartesian(), EnemyCharacter->GetAttackRadius()))
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
    // PlayerCharacter->TileSystem->UpdateLocation();
    // EnemyCharacter->TileSystem->UpdateLocation();


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
        if (CurrentCharacterSprite == PlayerCharacter->Get())
        {
            PlayerCharacter->SetDirection(CurrentCharacterSpriteDirection);
        }
        else
        {
            EnemyCharacter->SetDirection(CurrentCharacterSpriteDirection);
        }
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

void BattlemapScene::ProcessBackgroundScrolling()
{
    TextStream::instance().setText("BGX: " + std::to_string(BackgroundScroll.x), 3, 12);
    TextStream::instance().setText("BGY: " + std::to_string(BackgroundScroll.y), 4, 12);

    ScrollBackground(TileSystem->WorldOffset);

}

void BattlemapScene::ScrollBackground(TileSystemBase::TileCoordinates T)
{
    ScrollBackground(T.x*16, T.y*8);
}

void BattlemapScene::ScrollBackground(int x, int y)
{
    Battlemap->scroll(bglerpX+TileSystem->WorldOrigin.x+x, bglerpY+TileSystem->WorldOrigin.y+y);
    bg1->scroll(bglerpX+TileSystem->WorldOrigin.x+x, bglerpY+TileSystem->WorldOrigin.y+y);
    bg2->scroll(bglerpX+TileSystem->WorldOrigin.x+x, bglerpY+TileSystem->WorldOrigin.y+y);

    PlayerCharacter->MoveRelative({-x/16,-y/8});
    EnemyCharacter->MoveRelative({-x/16,-y/8});
    TileSystem->MoveRelative({-x/16,-y/8});
}



