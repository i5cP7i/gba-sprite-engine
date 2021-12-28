//
// Created by eduar on 12/28/2021.
//

#include "TileSelection.h"

TileSelection::TileSelection()
{
    std::unique_ptr<Sprite> TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(70, 121)
            .buildPtr();
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));

    TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(86, 121)
            .buildPtr();
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));

    TileSelectionSpriteVector.at(0).get()->setPalBank(2);
    TileSelectionSpriteVector.at(1).get()->setPalBank(2);

}

std::vector<Sprite*> TileSelection::Get() const
{
    std::vector<Sprite*> Temp;
    Temp.push_back(TileSelectionSpriteVector.at(0).get());
    Temp.push_back(TileSelectionSpriteVector.at(1).get());
    return Temp;
}

void TileSelection::Update()
{
    Get().at(0)->flipHorizontally(false);
    Get().at(1)->flipHorizontally(true);
    ShiftColor();
    // TileSelectionPalette->change(2, 0, PaletteManager::color(0x00,0x18,0xF7));
}

void TileSelection::ShiftColor()
{
    ColorShiftTimer++;
    if (ColorShiftTimer >= 7)
    {
        unsigned short Temp = TileSelectionColors[0];
        for (size_t i= 0; i < (sizeof(TileSelectionColors)/2 - 1); ++i)
        {
            TileSelectionColors[i] = TileSelectionColors[i+1];
        }
        TileSelectionColors[sizeof(TileSelectionColors)/2-1] = Temp;

        for (size_t i = 0; i < 10; ++i)
        {
            pal_obj_mem[32+i] = TileSelectionColors[ColorShift];
            if (ColorShift >= (sizeof(TileSelectionColors)/2-1))
            {
                ColorShift = 0;
            }
            else
            {
                ColorShift++;
            }
        }
        ColorShiftTimer = 0;
    }
}
