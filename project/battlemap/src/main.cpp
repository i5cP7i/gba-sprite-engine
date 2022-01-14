#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/palette/palette_manager.h>
#include <libgba-sprite-engine/allocator.h>

#include "BattlemapScene.h"
#include <time.h>
/*
 * 16 bit color mode!!!!!!!!!!!!
 * grit bg.png -ftc -mRtpf -gB4 -pe16 -pu16
 */

int main()
{
    std::shared_ptr<GBAEngine> engine(new GBAEngine());

    BattlemapScene* startScene = new BattlemapScene(engine);
    engine->setScene(startScene);

    srand (time(NULL));

    while (true) {
        engine->update();
        // engine->delay(1000);
    }

    return 0;
}

