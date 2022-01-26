//
// Created by eduar on 1/12/2022.
//

#include "MenuManager.h"

void MenuManager::Draw(MenuObject::vi2d vScreenOffset)
{
    if (panels.empty())
    {
        bOpen = false;
        return;
    }
    bOpen = true;
    for (auto &p : panels)
    {
        p->DrawSelf(vScreenOffset);
        vScreenOffset += {8, 2};
    }
}

MenuObject *MenuManager::OnConfirm()
{
    if (panels.empty())
    {
        return nullptr;
    }

    MenuObject* next = panels.back()->OnConfirm();

    if (next == panels.back())
    {
        if (panels.back()->GetSelectedItem()->isEnabled())
        {
            return panels.back()->GetSelectedItem();
        }

    }
    else
    {
        if (next->isEnabled())
        {
            panels.push_back(next);
        }
    }
    return nullptr;
}
