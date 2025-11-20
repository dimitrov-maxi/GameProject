#ifndef GAMEPROJECT_ACTIONS_H
#define GAMEPROJECT_ACTIONS_H
#include "../Entities/AllEntities.h"

class Action {
public:
    virtual bool execute(Entity &user, Entity &target) = 0; // pure virtual
    virtual ~Action() = default;
};

class LightAttack : public Action {
public:
    bool execute(Entity &user, Entity &target) override;
};

class HeavyAttack : public Action {
public:
    bool execute(Entity &user, Entity &target) override;
};

class Heal : public Action {
public:
    bool execute(Entity &user, Entity &target) override;
};
class Rest : public Action {
public:
    bool execute(Entity &user, Entity &target) override;
};
class Guard : public Action {
public:
    bool execute(Entity &user, Entity &target) override;
};

#endif //GAMEPROJECT_ACTIONS_H
