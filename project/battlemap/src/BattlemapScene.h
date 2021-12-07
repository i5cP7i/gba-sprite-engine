//
// Created by eduar on 12/7/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H

#include <libgba-sprite-engine/scene.h>

class BattlemapScene : public Scene
{
private:

public:
    BattlemapScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
};
#endif //GBA_SPRITE_ENGINE_PROJECT_BATTLEMAPSCENE_H
