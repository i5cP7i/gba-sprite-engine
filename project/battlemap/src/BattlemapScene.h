//
// Created by eduar on 12/7/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H

#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include <algorithm>
#include <random>

#include "Player.h"
#include "Enemy.h"
#include "TileSystemBase.h"
#include "ForegroundPalette.h"
#include "MenuObject.h"
#include "MenuManager.h"

// #include "test.h"

// #define _DEBUGMODE_0

class BattlemapScene : public Scene
{
private:
    std::unique_ptr<Background> Battlemap;

    // std::unique_ptr<Background> Text;
    std::unique_ptr<Background> bg1;
    std::unique_ptr<Background> bg2;

    std::unique_ptr<Player> PlayerCharacter;
    std::unique_ptr<Enemy> EnemyCharacter;

    MenuObject MenuSystem;
    MenuManager MenuSystemManager;
    MenuObject::vi2d MenuScreenOffset = {1,4};

    std::unique_ptr<TileSystemBase> TileSystem;

    enum class eGameState {Setup, Play, End, Reset, Quit} GameState;

    bool isGameOver();

    u16 prev_keys = 0;

    bool isGameStateTransitioning (unsigned Delay);

    void Setup(u16 keys);
    void Play(u16 keys);
    void End(u16 keys);
    void Reset();

    // Sprites
    Sprite* CurrentCharacterSprite;
    Sprite* OtherCharacterSprite;
    Sprite* WinningSprite;
    Sprite* LosingSprite;
    CharacterBase::eDirection CurrentCharacterSpriteDirection;
    CharacterBase::eDirection OtherCharacterSpriteDirection;

    int TurnID = 0;
    bool bGameOver = false;
    bool OnEndState = false;
    int oldTurnID = TurnID;

    // Gameplay MenuObject Functions
    enum class eGameMenu {Init = 0, Move = 1, Attack = 2, Items = 3, Wait = 4} GameMenu = eGameMenu::Init;
    // enum class eMenuSelect {Init, Move, Action, Wait} MenuSelect;

    void Menu(u16 keys);
    void InitMenu(u16 keys);
    void MoveMenu(u16 keys);
    void AttackMenu(u16 keys);
    void ItemMenu(u16 keys);
    void WaitMenu(u16 keys);

    // Gameplay MenuObject variables
    unsigned char MenuSelected = 0;
    unsigned char GameMenuSelected = 0;

    // MenuObject Aux Functions
    unsigned char ClipValue(unsigned char Number, unsigned char LowerBound, unsigned char UpperBound);;


    bool GetKeyState(u16 keys, u16 key);
    bool isKeyReleased(u16 keys, u16 key);
    bool isKeyPressed(u16 keys, u16 key);

    u16 old_key;
    bool StartKeyPressed = false;
    bool isStartKeyRising(u16 keys);
    bool RightKeyPressed = false;
    bool isRightKeyRising(u16 keys);
    bool LeftKeyPressed = false;
    bool isLeftKeyRising(u16 keys);
    bool UpKeyPressed = false;
    bool isUpKeyRising(u16 keys);
    bool DownKeyPressed = false;
    bool isDownKeyRising(u16 keys);
    bool AKeyPressed = false;
    bool isAKeyRising(u16 keys);
    bool BKeyPressed = false;
    bool isBKeyRising(u16 keys);

    #ifdef _DEBUGMODE_0
    std::unique_ptr<TileSystem> TileSelector;
    SpriteBuilder<Sprite> Builder;
    std::unique_ptr<Sprite> playertest;
    #endif

    // Background scroll functions and variables
    std::vector<TileSystemBase::TileCoordinates> TileReference;
    TileSystemBase::TileCoordinates BackgroundScroll;
    TileSystemBase::TileCoordinates CurrentTileLocation;
    TileSystemBase::TileCoordinates PreviousTileLocation;

    bool offsetSprites = false;
    int m_offsetX = 0; // offsetX = (m_offsetX % TileWidth)/ TileWidth
    int m_offsetY = 0; // offsetY = (m_offsetY % TileHeight)/ TileHeight
    TileSystemBase::TileCoordinates bgOffset = {0,0};
    int bglerpX = 0;
    int bglerpY = 0;
    void ScrollBackground(TileSystemBase::TileCoordinates T);
    void ScrollBackground(int x, int y);
    void ProcessBackgroundScrolling();
public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    BattlemapScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};
#endif //GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
