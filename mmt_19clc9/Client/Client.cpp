#include "Header.h"

int main() {
    struct sockaddr_in addrport;
    struct sockaddr_in* server = NULL, * result = NULL;
    client_type client;
    std::string msg = "";
    int iResult = 0;

    std::string str;
    std::cout << "Input server ip address: ";
    getline(std::cin, str);
    const char* c = str.c_str();

    std::cout << "Intializing Winsock..." << std::endl;

    WSADATA wsaData;
    int wsOK = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsOK != 0) {
        std::cout << "Can't initialize winsock. Application is now exiting..." << std::endl;
        return 0;
    }

    ZeroMemory(&addrport, sizeof(addrport));
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(5000);
    addrport.sin_addr.s_addr = inet_addr(c);

    /*std::cout << "Setting up server..." << std::endl;
    getaddrinfo(IP_ADDRESS, PORT, &hints, &server);*/


    // Attempt to connect to an address until one succeeds
    while (true) {
        client.socket = socket(PF_INET, SOCK_STREAM, 0);
        iResult = connect(client.socket, (struct sockaddr*)&addrport, sizeof(addrport));
        if (iResult == SOCKET_ERROR) {
            closesocket(client.socket);
            client.socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    if (client.socket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    Register(client);
    //int iSuccess = Login(client);

    while (true) {
        if (Login(client) == true) {
            std::cout << "Successfully Connected" << std::endl;
            break;
        }
    }

    Client_Group_Chat(client);

    std::cout << "Shutting down socket..." << std::endl;

    shutdown(client.socket, SD_SEND);
    closesocket(client.socket);
    WSACleanup();
    system("pause");
    return 0;
}