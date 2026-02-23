#include "CustomFunctions.h"
#include "Action.h"

namespace fs = std::filesystem;

bool CustomFunctions::randomBool() {
    return rand() % 2;
}

Monster CustomFunctions::choseEnemy() {
    Monster m;

    short enemyChoice;

    while (true) {
        std::cout << "Choose your enemy:\n"
                  << "1 | Slime (lv1)\n"
                  << "2 | Skeleton (lv2)\n"
                  << "3 | Goblin (lv3)\n"
                  << "4 | Troll (lv4)\n"
                  << "5 | Boss (lv5)\n";

        std::cout << "Enter choice: ";

        if (std::cin >> enemyChoice && enemyChoice >= 1 && enemyChoice <= 5) {
            break;
        }
        std::cout << "Invalid input! Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    switch (enemyChoice) {
        case 1: m.loadBinary("Slime");    break;
        case 2: m.loadBinary("Skeleton"); break;
        case 3: m.loadBinary("Goblin");   break;
        case 4: m.loadBinary("Troll");    break;
        case 5: m.loadBinary("Boss");     break;
    }
    return m;
}
std::vector<std::string> CustomFunctions::listSaveFiles() {
    std::vector<std::string> saves;

    for (const auto &entry: std::filesystem::directory_iterator("Saves/Players/")) {
        if (entry.is_regular_file()) {
            if (entry.path().extension() == ".bin") {
                saves.push_back(entry.path().stem().string());
            }
        }
    }
    return saves;
}

std::string CustomFunctions::getLocalIP() {
#ifdef _WIN32
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS) {
        return "Unknown";
    }

    PIP_ADAPTER_INFO pAdapter = AdapterInfo;
    while (pAdapter) {
        if (strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") != 0) {
            return std::string(pAdapter->IpAddressList.IpAddress.String);
        }
        pAdapter = pAdapter->Next;
    }

    return "Unknown";
#else
    return "Unsupported";
#endif
}

void CustomFunctions::initNetwork() {
#ifdef _WIN32
    WSADATA wsData;
    WSAStartup(MAKEWORD(2, 2), &wsData);
#endif
}

void CustomFunctions::closeSocket(int sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    ::close(sock);
#endif
}

bool CustomFunctions::convertIp(const std::string &ipStr, sockaddr_in &addr) {
#ifdef _WIN32
    addr.sin_addr.s_addr = inet_addr(ipStr.c_str());
    return addr.sin_addr.s_addr != INADDR_NONE;
#else
    return inet_pton(AF_INET, ipStr.c_str(), &addr.sin_addr) == 1;
#endif
}
int CustomFunctions::startHost() {
    const int port = 5500;
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Cannot create socket\n";
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSock, (sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed\n";
        closeSocket(serverSock);
        return -1;
    }

    listen(serverSock, 1);
    std::cout << "Waiting for a device to connect..." << std::endl;

    sockaddr_in clientAddr{};
#ifdef _WIN32
    int clientSize = sizeof(clientAddr);
#else
    socklen_t clientSize = sizeof(clientAddr);
#endif

    int clientSock = accept(serverSock, (sockaddr *) &clientAddr, &clientSize);
    if (clientSock < 0) {
        std::cerr << "Accept failed\n";
        closeSocket(serverSock);
        return -1;
    }

    std::cout << "Client connected!" << std::endl;
    closeSocket(serverSock);
    return clientSock;
}

int CustomFunctions::connectToHost(const std::string &ip) {
    const int port = 5500;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Cannot create socket\n";
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (!convertIp(ip, serverAddr)) {
        std::cerr << "Invalid IP address!\n";
        closeSocket(sock);
        return -1;
    }

    std::cout << "Connecting..." << std::endl;
    if (connect(sock, (sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        closeSocket(sock);
        return -1;
    }

    std::cout << "Connected to host!" << std::endl;
    return sock;
}


void CustomFunctions::sendAction(int sock, short action) {
    uint16_t networkValue = htons(static_cast<uint16_t>(action));
    send(sock, reinterpret_cast<char *>(&networkValue), sizeof(networkValue), 0);
}

short CustomFunctions::receiveAction(int sock) {
    uint16_t networkValue = 0;
    int bytesReceived = recv(sock, reinterpret_cast<char *>(&networkValue), sizeof(networkValue), 0);
    if (bytesReceived != sizeof(networkValue)) return -1;
    return static_cast<short>(ntohs(networkValue));
}
void CustomFunctions::sendPlayer(int sock, Player p) {
    std::string message = std::to_string(p.getLevel()) + "|" + p.getName();
    send(sock, message.c_str(), static_cast<int>(message.size() + 1), 0);
}

Player CustomFunctions::receivePlayer(int sock) {
    Player p = Player();

    char buffer[256]{};
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Problem receiving entity data!\n";
    }
    buffer[bytesReceived] = '\0';

    std::string msg(buffer);
    size_t sep = msg.find('|');
    if (sep == std::string::npos) {
        std::cerr << "Problem receiving entity data!\n";
    }
    for (short i = 0; i < static_cast<short>(std::stoi(msg.substr(0, sep))) - 1; i++) {
        p.levelUp(true);
    }
    p.setName(msg.substr(sep + 1).data());
    return p;
}

void CustomFunctions::Fight(Player p, Player p2, bool firstTurn, int socket, Action *playerActions[5]) {
    short counter = 1;
    system("cls");
    while (p.isAlive() && p2.isAlive()) {
        int choice;
        if (firstTurn) {
            std::cout << "Turn: " << counter << std::endl;
            p.printStats();
            p2.printStats();
            std::cout << "Choose action:\n"
                    "1-Light attack(-10 energy, strength x1)\n"
                    "2-Heavy attack(-20 energy, strength x2)\n"
                    "3-Heal(-25 energy, +20 health)\n"
                    "4-Guard(+10 energy, guarding stance)\n"
                    "5-Rest(+30energy,+10 health)" << std::endl;
            do {
                while (!(std::cin >> choice)) {
                    std::cout << "Enter numbers!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                while (choice < 1 || choice > 5) {
                    std::cout << "Invalid action, choose again!" << std::endl;
                    std::cin >> choice;
                }
            } while (!playerActions[choice - 1]->execute(p, p2));
            sendAction(socket, choice - 1);
            firstTurn = false;
        } else {
            p.printStats();
            p2.printStats();
            std::cout << "Waiting for enemy turn" << std::endl;
            int action = receiveAction(socket);
            playerActions[action]->execute(p2, p);
            firstTurn = true;
        }
        std::cout << "Press Enter to continue";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        system("cls");
        counter++;
    }
}
