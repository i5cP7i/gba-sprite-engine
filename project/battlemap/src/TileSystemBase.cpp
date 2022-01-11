//
// Created by eduar on 12/28/2021.
//

#include "TileSystemBase.h"

TileSystemBase::TileSystemBase()
{
    std::unique_ptr<Sprite> TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(240, 180)
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
    SetTileStatus(eStatus::Inactive);
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
    if (TileStatus == eStatus::Valid || TileStatus == eStatus::Invalid)
    {
        ColorShiftTimer++;
        if (ColorShiftTimer >= 7)
        {
            unsigned short Temp = TileColors[0];
            for (size_t i = 0; i < (sizeof(TileColors) / 2 - 1); ++i)
            {
                TileColors[i] = TileColors[i + 1];
            }
            TileColors[sizeof(TileColors) / 2 - 1] = Temp;

            for (size_t i = 0; i < 10; ++i)
            {
                pal_obj_mem[32+i] = TileColors[ColorShift];
                if (ColorShift >= (sizeof(TileColors) / 2 - 1))
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
    else
    {
        ColorShiftTimer = 0;
        for (size_t i = 0; i < 10; ++i)
        {
            pal_obj_mem[32+i] = 0xF000;
        }

    }
}

void TileSystemBase::Move(int x, int y)
{
    this->x = x;
    this->y = y;
    for (int i = 0; i < TileSelectionSpriteVector.size() / 2; i += 2)
    {
        TileSelectionSpriteVector.at(i)->moveTo(x, y);
        TileSelectionSpriteVector.at(i+1)->moveTo(x+16, y);
    }

}

void TileSystemBase::MoveRight()
{

    Move(x+2*8, y+8);
}

void TileSystemBase::MoveLeft()
{
    Move(x-2*8, y-8);
}

void TileSystemBase::MoveUp()
{
    Move(x+2*8, y-8);
}

void TileSystemBase::MoveDown()
{
    Move(x-2*8, y+8);
}

void TileSystemBase::SetTileStatus(TileSystemBase::eStatus Status)
{
    this->TileStatus = Status;
    switch (TileStatus)
    {
        case eStatus::Valid:
            for (size_t i = 0; i < 7 ; ++i)
            {
                TileColors[i] = TileColorSet[i];
            }
            break;
        case eStatus::Invalid:
            for (size_t i = 7; i < 14 ; ++i)
            {
                TileColors[i-7] = TileColorSet[i];
            }
            break;
        case eStatus::Inactive:
        default:
            break;
    }
}
