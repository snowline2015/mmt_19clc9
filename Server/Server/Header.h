#ifndef HEADER_H
#define HEADER_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <ctime>
#pragma comment( lib, "wsock32.lib" )
#pragma comment( lib, "Ws2_32.lib")
#pragma comment( lib, "Winmm.lib")
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

#define DEFAULT_MSG_LENGTH 4096
#define DEFAULT_TRANSFER_BUFFER_SIZE 512

const char OPTION_VALUE = 1;
const int MAX_CLIENTS = 10;

struct client_type
{
    int id = -1;
    volatile bool Online = false;
    std::string Username, Password, Fullname, DOB, Email, Bio, RoomID;
    SOCKET socket = INVALID_SOCKET;
};

void CloseSocket(SOCKET Sockid);
void Client_Thread(SOCKET NewSockid, std::vector<client_type>& client_List, std::vector<client_type>& client, std::thread my_thread[], int temp_id, std::thread& thread);
void Client_Multiple_Chatting(client_type& new_client, std::vector<client_type>& client_array, std::thread& thread);
void Client_Single_Chatting(client_type& first_client, std::vector<client_type>& client_array, std::string second_username, std::thread& thread);
void Read_Account(std::vector<client_type>& User_List);
void Write_Account(std::vector<client_type>& User_List);
bool Register(SOCKET NewSockid, std::vector<client_type>& User_List);
bool Login(SOCKET NewSockid, std::vector<client_type>& User_List, string& username);
void Check_Users_Online(SOCKET NewSockid, std::vector<client_type> User_List);
void Change_Password(SOCKET NewSockid, std::vector<client_type>& client_List);
void Change_Info(SOCKET NewSockid, std::vector<client_type>& client_List);
void Check_User(SOCKET NewSockid, std::vector<client_type> client, std::vector<client_type> client_List);

bool Upload_File(client_type& client, std::string& fileName);
bool Download_File(client_type& client, std::string& fileName);

#endif
