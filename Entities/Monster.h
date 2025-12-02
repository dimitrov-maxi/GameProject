//
// Created by dimit on 11/8/2025.
//

#ifndef GAMEPROJECT_MONSTER_H
#define GAMEPROJECT_MONSTER_H
#include "Entity.h"


class Monster:public Entity {
public:
    std::string getSaveLocation() override { return "Saves/Monsters/"; }
};


#endif //GAMEPROJECT_MONSTER_H