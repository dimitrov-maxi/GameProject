#include <chrono>
#include <thread>

#include "../Entities/AllEntities.h"
#include "Action.h"

LightAttack light;
HeavyAttack heavy;
Heal heal;
Guard guard;
Rest rest;

Action* playerActions[] = { &light, &heavy, &heal, &guard, &rest };


void runGame() {
    srand(time(nullptr));
    auto p1 = Player();
    auto e1 = Monster();
    e1.setName("Boss 1");
    e1.levelUp();
    std::cout << "What is your hero name?" << std::endl;
    char buff[20];
    std::cin >> buff;
    p1.setName(buff);
    short counter = 1;
    system("cls");
    while (p1.isAlive() && e1.isAlive()) {
        std::cout<<"Turn: "<<counter<<std::endl;
        p1.printStats();
        e1.printStats();
        std::cout<<"Choose action:\n"
                   "1-Light attack(-10 energy, strength x1)\n"
                   "2-Heavy attack(-20 energy, strength x2)\n"
                   "3-Heal(-25 energy, +20 health)\n"
                   "4-Guard(+10 energy, guarding stance)\n"
                   "5-Rest(+30energy,+10 health)"<<std::endl;
        int choice;
        do {
            std::cin >> choice;
            while (choice<1||choice>5) {
                std::cout<<"Invalid action, choose again!"<<std::endl;
                std::cin>>choice;
            }
        }while (!playerActions[choice-1]->execute(p1, e1));
        if (e1.isAlive()) {
            auto action = EntityBehaviour::entityFightBehaviour(e1);
            action->execute(e1, p1);
            delete action;

            // int enemyChoice;
            // do {
            //     enemyChoice = rand() % 5;
            // }while (!playerActions[enemyChoice]->execute(e1, p1));
            counter++;
        }
        std::cout<<"Press Enter to continue";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // to clean cin
        std::cin.get();
        system("cls");
    }
    if (p1.isAlive()) {
        std::cout << p1.getName()<<" has saved the princess";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }else {
        std::cout << e1.getName()<<" has killed the hero";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
