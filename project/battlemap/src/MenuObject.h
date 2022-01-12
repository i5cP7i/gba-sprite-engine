//
// Created by eduar on 1/12/2022.
// Adapted from OneLoneCoder's Retro MenuObject pop-up system.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MENUOBJECT_H
#define GBA_SPRITE_ENGINE_PROJECT_MENUOBJECT_H

#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_math.h>
#include <unordered_map>
#include <algorithm>
#include <vector>

class MenuObject
{
public:
    struct vi2d
    {
        int x;
        int y;

        void operator +=(vi2d v)
        {
            x += v.x;
            y += v.y;
        }

        void operator -=(vi2d v)
        {
            x -= v.x;
            y -= v.y;
        }
    };
private:
    const int nPatch = 8;
protected:
    std::string sName;
    bool bEnabled = true;
    int nID = -1;
    int nTotalRows = 0;
    vi2d vCellTable = {1, 0};
    vi2d vCellSize = {0, 0};
    vi2d vCellPadding = {2, 0};

    const vi2d vPatchSize = {nPatch, nPatch};
    vi2d vSizeInPatches = {0, 0};

    std::unordered_map<std::string, size_t> itemPointer; // std::map ORDERS THE ITEMS ALPHABETICALLY!!
    std::vector<MenuObject> items;

    vi2d vCellCursor = {0,0};
    int nCursorItem = 0;
    vi2d vCursorPosition = {0,0};

    int ClipValue(int Number, int LowerBound, int UpperBound)
    {
        return std::max(LowerBound, std::min(Number, UpperBound));
    }

public:
    MenuObject() : sName("root")
    {

    }
    explicit MenuObject(const std::string& Name)
    {
        this->sName = Name;
    }

    MenuObject& SetTable(int Columns, int Rows)
    {
        vCellTable = {Columns, Rows};
        return *this;
    }

    MenuObject& SetID(int id)
    {
        nID = id;
        return *this;
    }

    MenuObject& Enable(bool bEnable)
    {
        bEnabled = bEnable;
        return *this;
    }

    bool isEnabled() const { return bEnabled; }

    vi2d GetSize() const // Size required to Display sName of menu item.
    {
        return {int(sName.size()), 1};
    }

    std::string& GetName() // Size required to Display sName of menu item.
    {
        return sName;
    }

    int& GetID() // Size required to Display sName of menu item.
    {
        return nID;
    }

    vi2d& GetCursorPosition()
    {
        return vCursorPosition;
    }

    bool HasChildren()
    {
        return !items.empty();
    }

    MenuObject& operator[](const std::string& Name)
    {
        if (itemPointer.count(Name) == 0)
        {
            itemPointer[Name] = items.size();
            items.push_back(MenuObject(Name));
        }
        return items[itemPointer[Name]];
    }

    void Build();
    void DrawSelf(vi2d ScreenOffset);

    void OnUp();
    void OnDown();
    MenuObject* OnConfirm();
    MenuObject* GetSelectedItem() { return &items[nCursorItem]; }
    void ClampCursor();

    std::string sLastAction;
};


#endif //GBA_SPRITE_ENGINE_PROJECT_MENUOBJECT_H
