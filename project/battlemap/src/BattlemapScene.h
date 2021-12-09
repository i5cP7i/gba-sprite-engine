//
// Created by eduar on 12/7/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class BattlemapScene : public Scene
{
private:
    std::unique_ptr<Background> Battlemap;

    std::unique_ptr<AffineSprite> player;

    int scrollX, scrollY;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    BattlemapScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};
#endif //GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
