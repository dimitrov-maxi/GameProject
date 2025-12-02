//
// Created by dimit on 11/8/2025.
//

#include "Entity.h"

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

Entity::Entity(){};

short Entity::getLevel() {
    return level;
}

void Entity::heal(int amount) {
    if (maxHealth-currentHealth > amount) {
        currentHealth += amount;
    }else {
        currentHealth = maxHealth;
    }
    std::cout<<name << " has healed to "<< currentHealth << " health!"<<std::endl;
}
void Entity::modifyEnergy(int amount) {
    if (amount>0) {
        if (maxEnergy-currentEnergy > amount) {
            currentEnergy += amount;
        }else {
            currentEnergy = maxEnergy;
        }
        std::cout<<name << " has restored to "<< currentEnergy << " energy!"<<std::endl;
    }else {
        currentEnergy += amount;
        if (currentEnergy < 0) currentEnergy = 0;
            std::cout<<name << " has lost "<< amount << " energy and now has "<<currentEnergy<<std::endl;
        }
    }
bool Entity::getStance() {
    return stance;
}
void Entity::setStance(bool newStance) {
    std::cout << name<< " is guarding"<< std::endl;
    stance = newStance;
}
int Entity::attack(bool type) {
    std::cout << name<< " is attacking"<< std::endl;
    stance = 0;
    if (type) {
            return strength*2;
    }else{
            return strength;
    }
}

void Entity::takeDamage(int damage) {
    if (stance) {
        armor+=5;
    }
    if (currentHealth+armor>damage) {
        currentHealth-=damage-armor;
        std::cout <<name << " has taken " << damage-armor << " damage and has " << currentHealth << " health left!" <<std::endl;
    }else {
        status = 0;
        std::cout <<name<< " has died!" << std::endl;
    }
    if (stance) {
        armor-=5;
    }
};
void Entity::setName(char* newName) {
    delete[] name;
    short len = 0;
    while (newName[len] != '\0') {
        len++;
    }
    name = new char[len+1];
    for (short i = 0; i < len; i++) {
        name[i] = newName[i];
    }
    name[len] = '\0';
};
char* Entity::getName() {
    return name;
}
bool Entity::isAlive() {
    return status;
}
double Entity::getEnergy() {
    return currentEnergy;
}
double Entity::getMaxEnergy() {
    return maxEnergy;
}
double Entity::getHealth() {
    return currentHealth;
}
double Entity::getMaxHealth() {
    return maxHealth;
}
void Entity::printStats() {
    double percent = ((currentHealth) / maxHealth) * 100.0;
    short fillament = round(percent/10);
    std::cout << name<<": \nHealth[";
    for (short i = 0; i < 10; i++) {
        if (i<fillament) {
            std::cout<<"#";
        }else {
            std::cout<<" ";
        }

    }
    std::cout << "]"<< currentHealth <<"/"<<maxHealth <<std::endl;
    percent = ((currentEnergy) / maxEnergy) * 100.0;
    fillament = round(percent/10);
    std::cout <<"Energy[";
    for (short i = 0; i < 10; i++) {
        if (i<fillament) {
            std::cout<<"#";
        }else {
            std::cout<<" ";
        }

    }
    std::cout << "]"<< (int)currentEnergy <<"/"<<maxEnergy <<std::endl;
}
void Entity::gainExperience(double experienceGained) {
    experience += experienceGained;
    std::cout << name <<" has earned "<< experienceGained<< " Experience!"<<std::endl;
    while (experience>=experienceToLevel) {
        experience-=experienceToLevel;
        levelUp();
    }
    std::cout << experience << "out of " << experienceToLevel<<std::endl;
}
void Entity::levelUp(bool silent) {
    level++;
    if (!silent) {
        std::cout << name<<" has gained a level and now is level: "<<level<<std::endl;
    }
    experienceToLevel = experienceToLevel*1.2;
    maxHealth = maxHealth*1.2;
    currentHealth = maxHealth;
    maxEnergy = maxEnergy*1.1;
    currentEnergy = maxEnergy;
    strength = strength*1.2;
}
void Entity::saveBinary(std::string filename){
    std::ofstream file(getSaveLocation() + filename + ".bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: "
                  << getSaveLocation() << filename << ".bin" << std::endl;
        return;
    }
    file.write((char*)&level, sizeof(level));

    short len = strlen(name);
    file.write((char*)&len, sizeof(len));
    file.write(name, len);

    file.write((char*)&maxHealth, sizeof(maxHealth));
    file.write((char*)&currentHealth, sizeof(currentHealth));
    file.write((char*)&maxEnergy, sizeof(maxEnergy));
    file.write((char*)&currentEnergy, sizeof(currentEnergy));
    file.write((char*)&strength, sizeof(strength));
    file.write((char*)&armor, sizeof(armor));
    file.write((char*)&stance, sizeof(stance));
    file.write((char*)&status, sizeof(status));
    file.write((char*)&experience, sizeof(experience));
    file.write((char*)&experienceToLevel, sizeof(experienceToLevel));
    file.close();
}
void Entity::loadBinary(std::string filename){
    std::ifstream file(getSaveLocation() + filename + ".bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: "
                  << getSaveLocation() << filename << ".bin" << std::endl;
        return;
    }
    file.read((char*)&level, sizeof(level));

    short len;
    file.read((char*)&len, sizeof(len));

    char* buffer = new char[len + 1];
    file.read(buffer, len);
    buffer[len] = '\0';
    setName(buffer);
    delete[] buffer;

    file.read((char*)&maxHealth, sizeof(maxHealth));
    file.read((char*)&currentHealth, sizeof(currentHealth));
    file.read((char*)&maxEnergy, sizeof(maxEnergy));
    file.read((char*)&currentEnergy, sizeof(currentEnergy));
    file.read((char*)&strength, sizeof(strength));
    file.read((char*)&armor, sizeof(armor));
    file.read((char*)&stance, sizeof(stance));
    file.read((char*)&status, sizeof(status));
    file.read((char*)&experience, sizeof(experience));
    file.read((char*)&experienceToLevel, sizeof(experienceToLevel));
    file.close();
}
