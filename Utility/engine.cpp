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
    std::cout<<"\n1. New game\n2. Load save"<<std::endl;
    short newGame;
    std::cin>>newGame;
    Player p;
    if (newGame==1) {
        p = Player();
        std::cout << "What is your hero name?" << std::endl;
        char buff[20];
        std::cin >> buff;
        p.setName(buff);
    }else if(newGame == 2) {
        std::cout << "Choose save:\n";

        auto saves = CustomFunctions::listSaveFiles();

        if (!saves.empty()) {
            for (int i = 0; i < saves.size(); i++) {
                std::cout << i + 1 << ". " << saves[i] << std::endl;
            }

            int saveChoice;
            std::cin >> saveChoice;

            while (saveChoice < 1 || saveChoice > saves.size()) {
                std::cout << "Invalid choice, try again: ";
                std::cin >> saveChoice;
            }
            p.loadBinary(saves[saveChoice - 1]);
            std::cout << "Loaded save: " << p.getName() << std::endl;
        }else {
            std::cout<<"No saves found!\nCreating new hero!"<<std::endl;
            p = Player();
            std::cout << "What is your hero name?" << std::endl;
            char buff[20];
            std::cin >> buff;
            p.setName(buff);
        }
    }
    std::cout<<"Choose your game mode:\n1. Single player\n2. Multiplayer"<<std::endl;
    short multiplayer;
    std::cin>>multiplayer;
    if (multiplayer==1) {
        p.heal(100);
        p.modifyEnergy(50);
        auto e1 = Monster();
        e1.loadBinary("boss1");

        short counter = 1;
        system("cls");
        while (p.isAlive() && e1.isAlive()) {
            std::cout<<"Turn: "<<counter<<std::endl;
            p.printStats();
            e1.printStats();
            std::cout<<"Choose action:\n"
                       "1-Light attack(-10 energy, strength x1)\n"
                       "2-Heavy attack(-20 energy, strength x2)\n"
                       "3-Heal(-25 energy, +20 health)\n"
                       "4-Guard(+10 energy, guarding stance)\n"
                       "5-Rest(+30energy,+10 health)"<<std::endl;
            int choice;
            do {
                while (!(std::cin>>choice)) {
                    std::cout<<"Enter numbers!"<<std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                while (choice<1||choice>5) {
                    std::cout<<"Invalid action, choose again!"<<std::endl;
                    std::cin>>choice;
                }
            }while (!playerActions[choice-1]->execute(p, e1));
            if (e1.isAlive()) {
                auto action = EntityBehaviour::entityFightBehaviour(e1);
                action->execute(e1, p);
                delete action;
                counter++;
            }
            std::cout<<"Press Enter to continue";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            system("cls");
        }
        if (p.isAlive()) {
            std::cout << p.getName()<<" has saved the princess";
            p.gainExperience(10 * (1.0 + e1.getLevel() * 0.1));
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }else {
            std::cout << e1.getName()<<" has killed the hero";
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        std::cout << "Do you want to save?(y/n)";
        if (std::cin.get()=='y') {
            p.saveBinary(p.getName());
        }else {

        }
    }else if (multiplayer==2) {
        CustomFunctions::initNetwork();
        char hostOrClient;
        std::cout<<"Host(h)/Connect(c)!"<<std::endl;
        while (!(std::cin>>hostOrClient)) {
            std::cout << "enter only one character"<<std::endl;
        }
        while (hostOrClient!='h'&&hostOrClient!='c') {
            std::cout<<"Invalid choice!"<<std::endl;
        }
        if (hostOrClient == 'h') {
            bool yourTurn = true;
            std::cout<<"Device ip is:" << CustomFunctions::getLocalIP();
            int socket = CustomFunctions::startHost();
            CustomFunctions::sendPlayer(socket, p);
            Player p2 = CustomFunctions::receivePlayer(socket);
            CustomFunctions::Fight(p,p2,true,socket,playerActions);
        }else {
            std::cout<<"Enter your buddy's IP:"<<std::endl;
            std::string ip;
            std::cin>>ip;
            int socket = CustomFunctions::connectToHost(ip);
            CustomFunctions::sendPlayer(socket, p);
            Player p2 = CustomFunctions::receivePlayer(socket);CustomFunctions::Fight(p,p2,false,socket,playerActions);
        }
    }
}
