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

#include "Player.h"
#include "Enemy.h"
#include "TileSystemBase.h"
#include "Plains.h"
#include "ForegroundPalette.h"

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

    std::unique_ptr<TileSystemBase> TileSystem;

    enum class eGameState {Setup, Play, End} GameState;

    bool isGameOver();

    u16 prev_keys = 0;
    void Setup(u16 keys);
    void Play(u16 keys);
    void End(u16 keys);
    void Reset(u16 keys);


    bool GetKeyState(u16 keys, u16 key);
    bool isKeyReleased(u16 keys, u16 key);
    bool isKeyPressed(u16 keys, u16 key);

    u16 old_key;
    bool RightKeyPressed = false;
    bool isRightKeyRising(u16 keys);
    bool LeftKeyPressed = false;
    bool isLeftKeyRising(u16 keys);
    bool UpKeyPressed = false;
    bool isUpKeyRising(u16 keys);
    bool DownKeyPressed = false;
    bool isDownKeyRising(u16 keys);

    #ifdef _DEBUGMODE_0
    std::unique_ptr<TileSystem> TileSelector;
    SpriteBuilder<Sprite> Builder;
    std::unique_ptr<Sprite> playertest;
    #endif
    int offsetX, offsetY;
public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    BattlemapScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};
#endif //GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
