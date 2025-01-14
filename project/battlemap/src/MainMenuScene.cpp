//
// Created by eduar on 1/25/2022.
//

#include "MainMenuScene.h"
#include "BattlemapScene.h"

void MainMenuScene::load()
{

    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(MainMenuBackgroundPal, sizeof(MainMenuBackgroundPal)));
    bg0 = std::unique_ptr<Background>(new Background(1, MainMenuBackgroundTiles, sizeof(MainMenuBackgroundTiles), MainMenuBackgroundMap, sizeof(MainMenuBackgroundMap),4,1, BG_REG_32x32));
    bg1 = std::unique_ptr<Background>(new Background(2, MainMenuBackgroundTiles, sizeof(MainMenuBackgroundTiles), MainMenuBackgroundMap, sizeof(MainMenuBackgroundMap),4,1, BG_REG_32x32));
    bg2 = std::unique_ptr<Background>(new Background(3, MainMenuBackgroundTiles, sizeof(MainMenuBackgroundTiles), MainMenuBackgroundMap, sizeof(MainMenuBackgroundMap),4,1, BG_REG_32x32));
    MenuSystem["main"].SetTable(1, 3);
    MenuSystem["main"]["Start"].SetID(100).Enable(true);
    MenuSystem.Build();

    MenuSystemManager.Open(&MenuSystem["main"]);
    TextStream::instance().setText("FINAL FANTASY TACTICS", 0, 5);
    TextStream::instance().setText("-CLONE-", 1, 18);
    engine->enqueueMusic(MainMenuMusic, sizeof(MainMenuMusic));
    // MenuSystemManager.Draw(MenuScreenOffset);
}

void MainMenuScene::tick(u16 keys)
{
    pal_bg_mem[15] = 0x2CEE;
    if (!bStart)
    {
        switch (MenuState)
        {
            case eMenuState::Init:
                InitMenu(keys);
                break;
            case eMenuState::Start:
                bStart = true;
                break;
            default:
                break;
        }
    }
    else
    {
        static unsigned countFade = 0;
        countFade++;
        if (!engine->isTransitioning() && countFade > 70)
        {
            countFade = 0;
            engine->transitionIntoScene(new BattlemapScene(engine), new FadeOutScene(1));
        }
    }

}

std::vector<Sprite *> MainMenuScene::sprites()
{
    return std::vector<Sprite *>();
}

std::vector<Background *> MainMenuScene::backgrounds()
{
    return
    {
            bg0.get(), bg1.get(), bg2.get(),
    };
}

void MainMenuScene::InitMenu(u16 keys)
{
    MenuObject *command = nullptr;
    static unsigned TextToggleCount = 0;
    TextToggleCount++;
    if (TextToggleCount >= 0 && TextToggleCount < 43)
    {
        TextStream::instance().setText("FINAL FANTASY TACTICS", 0, 5);
        TextStream::instance().setText("-CLONE-", 1, 18);
        TextStream::instance().setText("Push A to Start", 18, 6);
    }
    else if (TextToggleCount >= 43 && TextToggleCount < 86)
    {
        TextStream::instance().clear();
        TextStream::instance().setText("FINAL FANTASY TACTICS", 0, 5);
        TextStream::instance().setText("-CLONE-", 1, 18);
        TextStream::instance().setText("Push A to Start", 18, 10);
    }
    else if (TextToggleCount >= 86)
    {
        TextToggleCount = 0;
    }

    if (keys & KEY_A)
    {
        if (!MenuSystemManager.isOpen())
        {
            MenuSystemManager.Open(&MenuSystem["main"]);
        }
        else
        {
            command = MenuSystemManager.OnConfirm();
        }
    }
    if (command != nullptr)
    {
        // MenuSystem.sLastAction = "L.Act.: " + command->GetName() + "ID: " + std::to_string(command->GetID());
        MenuSystemManager.Close();
        TextStream::instance().setText(MenuSystem.sLastAction, 1, 1);
        switch (command->GetID())
        {
            case 100:
                engine->dequeueAllSounds();
                engine->enqueueSound(fade1, sizeof(fade1));
                bStart = true;
                break;
            default:
                break;
        }
        MenuSystemManager.OnBack();
    }
    MenuSystemManager.Draw(MenuScreenOffset);
}
