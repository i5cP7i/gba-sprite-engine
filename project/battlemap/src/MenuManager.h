//
// Created by eduar on 1/12/2022.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MENUMANAGER_H
#define GBA_SPRITE_ENGINE_PROJECT_MENUMANAGER_H

#include "MenuObject.h"
#include <list>

class MenuManager
{
private:
    std::list<MenuObject*> panels;
    bool bOpen = false;
public:
    MenuManager() = default;

    bool isOpen() const {return bOpen;}

    void Open(MenuObject* Menu) {Close(); panels.push_back(Menu);}
    void Close() {panels.clear(); TextStream::instance().clear();}
    void OnResetPosY()
    {
        if (!panels.empty())
        {
            while (panels.back()->GetCursorPosition().y != 0)
            {
                panels.back()->OnUp();
            }
        }
    }
    void OnUp() { if (!panels.empty()) panels.back()->OnUp();}
    void OnDown() { if (!panels.empty()) panels.back()->OnDown();}
    void OnBack()
    {
        if (!panels.empty())
        {
            panels.pop_back();
        }
        TextStream::instance().clear();
    }
    MenuObject* OnConfirm();

    void Draw(MenuObject::vi2d vScreenOffset);

};


#endif //GBA_SPRITE_ENGINE_PROJECT_MENUMANAGER_H
