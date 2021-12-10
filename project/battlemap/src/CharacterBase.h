//
// Created by eduar on 12/10/2021.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
#define GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H

class CharacterBase
{
private:
    std::unique_ptr<Sprite> Character;
protected:
    virtual void CreateSprite() = 0;
public:

};


#endif //GBA_SPRITE_ENGINE_PROJECT_CHARACTERBASE_H
