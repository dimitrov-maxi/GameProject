//
// Created by dimit on 11/8/2025.
//

#ifndef GAMEPROJECT_PLAYER_H
#define GAMEPROJECT_PLAYER_H
#include "Entity.h"


class Player: public Entity {
public:
    std::string getSaveLocation() override { return "Saves/Players/"; }
};

#endif //GAMEPROJECT_PLAYER_H