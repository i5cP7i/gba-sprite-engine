//
// Created by eduar on 1/25/2022.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MAINMENUSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MAINMENUSCENE_H

#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>

#include "MainMenuBackground.h"
#include "MenuObject.h"
#include "MenuManager.h"

#include "../music/MainMenuMusic.h"
#include "../sfx/fade1.h"

class MainMenuScene : public Scene
{
private:
    std::unique_ptr<Background> bg0;
    std::unique_ptr<Background> bg1;
    std::unique_ptr<Background> bg2;

    MenuObject MenuSystem;
    MenuManager MenuSystemManager;
    MenuObject::vi2d MenuScreenOffset = {12,14};


    void InitMenu(u16 keys);
    bool bStart = false;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    enum class eMenuState {Init, Start} MenuState;

    MainMenuScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_MAINMENUSCENE_H
