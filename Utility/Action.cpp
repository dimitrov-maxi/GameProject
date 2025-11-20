#include "Action.h"
#include <iostream>

bool LightAttack::execute(Entity &user, Entity &target) {
    if (user.getEnergy()>=10) {
        target.takeDamage(user.attack(0));
        user.modifyEnergy(-10);
        return 1;
    }else {
        std::cout<<"Not enough energy!!!"<<std::endl;
        return 0;
    }
}

bool HeavyAttack::execute(Entity &user, Entity &target) {
    if (user.getEnergy()>=20) {
        target.takeDamage(user.attack(1));
        user.modifyEnergy(-20);
        return 1;
    }else {
        std::cout<<"Not enough energy!!!"<<std::endl;
        return 0;
    }

}

bool Heal::execute(Entity &user, Entity &target) {
    if (user.getEnergy()>=25) {
        user.heal(20);
        user.modifyEnergy(-25);
        return 1;
    }else {
        std::cout<<"Not enough energy!!!"<<std::endl;
        return 0;
    }
}
bool Rest::execute(Entity &user, Entity &target) {
    user.modifyEnergy(30);
    user.heal(10);
    return 1;
}
bool Guard::execute(Entity &user, Entity &target) {
    user.setStance(1);
    user.modifyEnergy(10);
    return 1;
}
