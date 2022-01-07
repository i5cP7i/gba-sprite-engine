//
// Created by eduar on 12/28/2021.
//

#include "TileSystemBase.h"

TileSystemBase::TileSystemBase()
{
    std::unique_ptr<Sprite> TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(140, 121)
            .buildPtr();
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));

    TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(TileSelectionSpriteVector.at(0)->getX()+16, TileSelectionSpriteVector.at(0)->getY())
            .buildPtr();
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));

    for (int i = 0; i < TileSelectionSpriteVector.size(); ++i)
    {
        TileSelectionSpriteVector.at(i).get()->setPalBank(2);
    }
}

std::vector<Sprite*> TileSystemBase::Get() const
{
    std::vector<Sprite*> Temp;
    Temp.push_back(TileSelectionSpriteVector.at(0).get());
    Temp.push_back(TileSelectionSpriteVector.at(1).get());
    return Temp;
}

void TileSystemBase::Update()
{
    Get().at(0)->flipHorizontally(false);
    Get().at(1)->flipHorizontally(true);
    ShiftColor();
    // TileSelectionPalette->change(2, 0, PaletteManager::color(0x00,0x18,0xF7));
}

void TileSystemBase::ShiftColor()
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
