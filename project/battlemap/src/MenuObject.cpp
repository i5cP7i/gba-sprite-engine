//
// Created by eduar on 1/12/2022.
//

#include "MenuObject.h"

void MenuObject::Build()
{
    // Recursively build all children.
    for (auto &m: items)
    {
        if (m.HasChildren())
        {
            m.Build();
        }
        // The longest child name determines the cell width
        vCellSize.x = std::max(m.GetSize().x, vCellSize.x);
        vCellSize.y = std::max(m.GetSize().y, vCellSize.y);
    }
    // Adjust size of patches for drawing panel sprites.
    vSizeInPatches.x = vCellTable.x * vCellSize.x + (vCellTable.x - 1) * vCellPadding.x + 2;
    vSizeInPatches.y = vCellTable.y * vCellSize.y + (vCellTable.y - 1) * vCellPadding.y + 2;

    nTotalRows = (items.size() / vCellTable.x) + (((items.size() % vCellTable.x) > 0) ? 1 : 0);
}

void MenuObject::DrawSelf(vi2d ScreenOffset)
{
    vi2d vCell {0,0};
    for (auto &m : items)
    {
        if (vCellCursor.y == vCell.y)
        {
            TextStream::instance().setText(std::string("->") + m.GetName(),
                                           vCell.y+ScreenOffset.y,
                                           vCell.x+ScreenOffset.x);
        }
        else
        {
            TextStream::instance().setText(m.GetName(),
                                           vCell.y+ScreenOffset.y,
                                           vCell.x+ScreenOffset.x);
        }

        vCell.y++;
    }
}

void MenuObject::ClampCursor()
{
    // Find the item in children
    nCursorItem = vCellCursor.y;

    // Clamp the Cursor
    if (nCursorItem >= static_cast<int>(items.size()))
    {
        nCursorItem = items.size() - 1;
    }
}

void MenuObject::OnUp()
{
    vCellCursor.y = ClipValue(--vCellCursor.y, 0, items.size()-1);
    ClampCursor();
}

void MenuObject::OnDown()
{
    vCellCursor.y = ClipValue(++vCellCursor.y, 0, items.size()-1);
    ClampCursor();
}

MenuObject* MenuObject::OnConfirm()
{
    if (items[nCursorItem].HasChildren())
        return &items[nCursorItem];
    else
        return this;

}
