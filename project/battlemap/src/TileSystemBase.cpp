//
// Created by eduar on 12/28/2021.
//

#include "TileSystemBase.h"

TileSystemBase::TileSystemBase()
{
    std::unique_ptr<Sprite> TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(ResetX, ResetY)
            .buildPtr();
    TempTileSelectionSprite->buildOam(TempTileSelectionSprite->GetTileIndex(), 3);
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));


    TempTileSelectionSprite = TileSelectionBuilder
            .withData(TileSelectionTiles, sizeof(TileSelectionTiles))
            .withSize(SIZE_16_16)
            .withLocation(TileSelectionSpriteVector.at(0)->getX()+16, TileSelectionSpriteVector.at(0)->getY())
            .buildPtr();
    TempTileSelectionSprite->buildOam(TempTileSelectionSprite->GetTileIndex(), 3);
    TileSelectionSpriteVector.push_back(std::move(TempTileSelectionSprite));

    for (int i = 0; i < TileSelectionSpriteVector.size(); ++i)
    {
        TileSelectionSpriteVector.at(i).get()->setPalBank(2);
    }
    SetTileStatus(eStatus::Inactive);
    WorldOrigin = {8, 8};
}

std::vector<Sprite*> TileSystemBase::Get() const
{
    std::vector<Sprite*> Temp;
    Temp.push_back(TileSelectionSpriteVector.at(0).get());
    Temp.push_back(TileSelectionSpriteVector.at(1).get());
    return Temp;
}

void TileSystemBase::UpdateLocation()
{
    WorldLocation = GetWorldTransform(TileLocation);
    // WorldOffset = GetWorldTransformOffset(TileLocation);
    WorldCartesianLocation = GetWorldCartesian();
}

void TileSystemBase::Update()
{
    Get().at(0)->flipHorizontally(false);
    Get().at(1)->flipHorizontally(true);
    ShiftColor();
    UpdateLocation();

    // TextStream::instance().setText("WX: " + std::to_string(WorldLocation.x), 4, 12);
    // TextStream::instance().setText("WY: " + std::to_string(WorldLocation.y), 5, 12);
    // TextStream::instance().setText("OffWX: " + std::to_string(WorldLocation.x), 6, 12);
    // TextStream::instance().setText("OffWY: " + std::to_string(WorldLocation.y), 7, 12);
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
    // this->x = x;
    // this->y = y;
    TileLocation.x = x;
    TileLocation.y = y;

    for (int i = 0; i < TileSelectionSpriteVector.size() / 2; i += 2)
    {
        TileSelectionSpriteVector.at(i)->moveTo(x, y);
        TileSelectionSpriteVector.at(i+1)->moveTo(x+TileWidth/2, y);
    }

}

void TileSystemBase::Move(TileSystemBase::TileCoordinates T)
{
    Move(T.x * TileWidth/2, T.y * TileHeight/2);
}

void TileSystemBase::MoveRelative(TileSystemBase::TileCoordinates T)
{
    for (int i = 0; i < TileSelectionSpriteVector.size() / 2; i += 2)
    {
        TileSelectionSpriteVector.at(i)->moveTo(TileLocation.x+T.x*TileWidth/2, TileLocation.y+T.y*TileHeight/2);
        TileSelectionSpriteVector.at(i+1)->moveTo(TileLocation.x+T.x*TileWidth/2+TileWidth/2, TileLocation.y+T.y*TileHeight/2);
    }
}

void TileSystemBase::MoveRight()
{
    Move(TileLocation.x+TileWidth/2, TileLocation.y+TileHeight/2);
}

void TileSystemBase::MoveLeft()
{
    /*
    if (!(WorldLocation.x == 3 && WorldLocation.y == 13) && !(WorldLocation.x == 4 && WorldLocation.y == 12)
        && !(WorldLocation.x == 5 && WorldLocation.y == 11) && !(WorldLocation.x == 6 && WorldLocation.y == 10)
        && !(WorldLocation.x == 8 && WorldLocation.y == 10) && !(WorldLocation.x == 13 && WorldLocation.y == 13)
        && !(WorldLocation.x == 5 && WorldLocation.y == 17))
    */
    if (!(WorldCartesianLocation.x <= 8 && WorldCartesianLocation.y >= 2) && !(WorldCartesianLocation.x == 9 && WorldCartesianLocation.y == 1)
    && !(WorldCartesianLocation.x == 13 && WorldCartesianLocation.y <= 0))
    {
        Move(TileLocation.x-TileWidth/2, TileLocation.y-TileHeight/2);
    }

}

void TileSystemBase::MoveUp()
{
    /*
    if (!(WorldLocation.x == 6 && WorldLocation.y == 10) && !(WorldLocation.x == 8 && WorldLocation.y == 10)
        && !(WorldLocation.x == 9 && WorldLocation.y == 11) && !(WorldLocation.x == 10 && WorldLocation.y == 12)
        && !(WorldLocation.x == 11 && WorldLocation.y == 13) && !(WorldLocation.x == 13 && WorldLocation.y == 13))
    */
    if (!(WorldCartesianLocation.x <= 12 && WorldCartesianLocation.y == 1) && !(WorldCartesianLocation.x >= 13 && WorldCartesianLocation.y <= 0)
        && !(WorldCartesianLocation.x == 8 && WorldCartesianLocation.y <= 2))
    {
        Move(TileLocation.x+TileWidth/2, TileLocation.y-TileHeight/2);
    }

}

void TileSystemBase::MoveDown()
{
    /*
    if (!(WorldLocation.x == 3 && WorldLocation.y == 13) && !(WorldLocation.x == 4 && WorldLocation.y == 14)
         && !(WorldLocation.x == 5 && WorldLocation.y == 15) && !(WorldLocation.x == 5 && WorldLocation.y == 17))
    */
    if (!((WorldCartesianLocation.y == 5 && WorldCartesianLocation.x <=14)))
    {
        Move(TileLocation.x-TileWidth/2, TileLocation.y+TileHeight/2);
    }
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

TileSystemBase::TileCoordinates TileSystemBase::GetWorldCartesian() const
{
    return
    {
    (((GetWorldLocation().y-WorldOffset.y) + (GetWorldLocation().x-WorldOffset.x))/2),
    (((GetWorldLocation().y-WorldOffset.y) - (GetWorldLocation().x-WorldOffset.x))/2),
    };
}

