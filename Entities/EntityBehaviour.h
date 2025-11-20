//
// Created by dimit on 11/11/2025.
//

#ifndef GAMEPROJECT_ENTITYBEHAVIOUR_H
#define GAMEPROJECT_ENTITYBEHAVIOUR_H
#include <cstdlib>
#include <ctime>

#include "Entity.h"
// #include "../Utility/Action.h"
#include "../Utility/CustomFunctions.h"

class Action;
class EntityBehaviour {
public:
    bool isFriendly();

    static Action* entityFightBehaviour(Entity &entity);
};


#endif //GAMEPROJECT_ENTITYBEHAVIOUR_H