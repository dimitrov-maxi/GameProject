//
// Created by dimit on 11/8/2025.
//

#ifndef GAMEPROJECT_ENTITY_H
#define GAMEPROJECT_ENTITY_H
#include <iostream>


class Entity {
protected:
    short level = 1;
    char* name = nullptr;
    double maxHealth = 100;
    double currentHealth = 100;
    double maxEnergy = 100;
    double currentEnergy = 100;
    short armor = 0;
    short speed = 0;
    short luck = 0;
    bool status = 1;
    double strength = 10;
    double experience = 0;
    double experienceToLevel = 10;
    bool stance = 0;

public:
    Entity();
    short getLevel();
    void levelUp(bool silent = false);
    void heal(int health);
    void takeDamage(int damage);
    int attack(bool type);
    void setName(char* name);
    char* getName();
    bool isAlive();
    double getEnergy();
    void modifyEnergy(int amount);
    bool getStance();
    void setStance(bool stance);
    double getHealth();
    double getMaxHealth();
    double getMaxEnergy();
    void printStats();
    void gainExperience(double experienceGained);
    void saveBinary(std::string filename);
    void loadBinary(std::string filename);
    virtual std::string getSaveLocation() { return "Saves/"; } // default
};


#endif //GAMEPROJECT_ENTITY_H