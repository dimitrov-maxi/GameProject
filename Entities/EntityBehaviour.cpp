//
// Created by dimit on 11/11/2025.
//

#include "EntityBehaviour.h"
#include "../Utility/Action.h"
bool isFriendly() {
    return CustomFunctions::randomBool();
}
Action* EntityBehaviour::entityFightBehaviour(Entity &entity){
    double healthPercentage = (entity.getHealth()/entity.getMaxHealth())*100;
    double energyPercentage =( entity.getEnergy()/entity.getMaxEnergy())*100;
    if (healthPercentage<20) {
        if (energyPercentage>20) {
            return new Heal;
        }else {
            if (CustomFunctions::randomBool()) {
                return new Guard;
            }else {
                return new Rest;
            }
        }
    }else if (energyPercentage>20) {
        if (CustomFunctions::randomBool()) {
            return new HeavyAttack;
        }else {
            return new LightAttack;
        }
    }else {
        if (CustomFunctions::randomBool()) {
            return new Guard;
        }else {
            return new Rest;
        }
    }
}