#include "Header.h"

void Read_Account(std::vector<client_type>& User_List) {
    std::ifstream f("Data\\Account.csv");
    //SkipBOM(f);
    if (!f.is_open())
        return;
    string temp;
    while (!f.eof()) {
        client_type user;
        getline(f, user.Username, ',');
        getline(f, user.Password, '\n');
        if (user.Username.size() != 0)
            User_List.push_back(user);
    }
    f.close();
}

void Write_Account(vector<client_type>& users) {
    ofstream o;
    o.open("Data\Account.csv");
    if (!o.is_open())
        return;
    for (int i = 0; i < users.size(); ++i) {
        o << users[i].Username << ',';
        o << users[i].Password;
    }
    o.close();
}

void Client_Multiple_Chatting(client_type& new_client, std::vector<client_type>& client_array, std::thread& thread) {
    std::string msg = "";
    char tempmsg[DEFAULT_BUFFER_LENGTH] = "";

    while (true) {
        memset(&tempmsg, NULL, sizeof(tempmsg));

        if (new_client.socket != 0) {
            int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFFER_LENGTH, 0);
            //tempmsg[strlen(tempmsg)] = '\0';
            if (iResult != SOCKET_ERROR) {
                //if (strcmp("", tempmsg) == 0)
                    
                    
                    
                    msg = "Client #" + std::to_string(new_client.id) + ": " + (tempmsg);  // De y dong nay, client # la tam thoi
                    // Sau nay co username thi chinh sua lai



                //std::cout << msg.c_str() << std::endl;

                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_array[i].socket != INVALID_SOCKET)
                        if (client_array[i].socket == SOCKET_ERROR) continue;
                        if (new_client.id != i)
                            iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }
            }
            else {
                msg = "Client #" + std::to_string(new_client.id) + " has disconnected";

                PlaySound(TEXT("Sound\\Summoner.wav"), NULL, SND_ASYNC);

                std::cout << msg << std::endl;

                closesocket(new_client.socket);
                closesocket(client_array[new_client.id].socket);
                client_array[new_client.id].socket = INVALID_SOCKET;


                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_array[i].socket != INVALID_SOCKET)
                        iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }

                break;
            }
        }
    } 

    thread.detach();
}

bool Register(SOCKET NewSockid, std::vector<client_type>& User_List) {
    char temp[DEFAULT_BUFFER_LENGTH] = "";
    int iResult = recv(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
    if (iResult != SOCKET_ERROR) {
        for (std::vector<client_type>::iterator p = User_List.begin(); p != User_List.end(); p++) {
            if ((*p).Username.compare(std::string(temp)) == 0) {
                memset(&temp, NULL, sizeof(temp));
                strncpy(temp, "Username already taken", DEFAULT_BUFFER_LENGTH);
                send(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
                return false;
            }
        }
        send(NewSockid, "OK", DEFAULT_BUFFER_LENGTH, 0);
        client_type new_user;
        new_user.Username = std::string(temp);
        memset(&temp, NULL, sizeof(temp));
        iResult = recv(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
        if (iResult != SOCKET_ERROR) {
            new_user.Password = std::string(temp);
            User_List.push_back(new_user);
            memset(&temp, NULL, sizeof(temp));
            strncpy(temp, "Register successfully", DEFAULT_BUFFER_LENGTH);
            send(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
            return true;
        }
        return false;   
    }
    return false;
}

bool Login(SOCKET NewSockid, std::vector<client_type>& User_List) {
    char temp[DEFAULT_BUFFER_LENGTH] = "";
    int iResult = recv(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
    if (iResult != SOCKET_ERROR) {
        send(NewSockid, "OK", DEFAULT_BUFFER_LENGTH, 0);
        for (std::vector<client_type>::iterator p = User_List.begin(); p != User_List.end(); p++) {
            if ((*p).Username.compare(std::string(temp)) == 0) {
                memset(&temp, NULL, sizeof(temp));
                iResult = recv(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
                if (iResult != SOCKET_ERROR) 
                    if ((*p).Password.compare(std::string(temp)) == 0) {
                        memset(&temp, NULL, sizeof(temp));
                        strncpy(temp, "Login successfully", DEFAULT_BUFFER_LENGTH);
                        send(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
                        return true;
                    }
                    else {
                        memset(&temp, NULL, sizeof(temp));
                        strncpy(temp, "ID or Password is incorrect", DEFAULT_BUFFER_LENGTH);
                        send(NewSockid, temp, DEFAULT_BUFFER_LENGTH, 0);
                        return false;
                    }
                return false;
            }
        }
        return false;
    }
    return false;
}