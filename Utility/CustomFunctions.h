//
// Created by dimit on 11/11/2025.
//

#ifndef GAMEPROJECT_CUSTOMFUNCTIONS_H
#define GAMEPROJECT_CUSTOMFUNCTIONS_H
#include <ctime>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <string>
#include <cstring>
#include <iostream>

#include "Action.h"
#include "../Entities/Player.h"

#ifdef _WIN32
// #define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif

class Action;

class CustomFunctions {
     public:
     static bool randomBool();
     static Monster choseEnemy();
     static std::vector<std::string> listSaveFiles();
     static std::string getLocalIP();

     static void initNetwork();
     static void cleanupNetwork();
     static void closeSocket(int sock);

     static bool convertIp(const std::string& ipStr, sockaddr_in& addr);
     static int startHost();
     static int connectToHost(const std::string &ip);
     static void sendAction(int sock, short action);
     static short receiveAction(int sock);
     static void sendPlayer(int sock, Player p);
     static Player receivePlayer(int sock);

     static void Fight(Player p, Player p2, bool firstTurn, int socket, Action* playerActions[5]);
};


#endif //GAMEPROJECT_CUSTOMFUNCTIONS_H