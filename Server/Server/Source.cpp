#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )
#include <winsock.h>
#pragma comment( lib, "wsock32.lib" )
#define PORT 1806 


int main() {
	std::string str;
	/*std::cout << "Input client ip address: ";
	getline(std::cin, str);*/
	const char* c = str.c_str();

	int sockid = socket(PF_INET, SOCK_STREAM, 0), NewSockid;
	struct sockaddr_in addrport; 
	int AddrSize = sizeof(addrport);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(PORT);
	//inet_pton(AF_INET, "192.0.2.33", &(addrport.sin_addr));
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1"); //htonl(INADDR_ANY);
	bind(sockid, (struct sockaddr*)&addrport, sizeof(addrport));
	listen(sockid, SOMAXCONN);
	while (true) {
		NewSockid = accept(sockid, (struct sockaddr*)&addrport, &AddrSize);
		char* temp = new char[200];
		if (recv(NewSockid, temp, sizeof(temp), 0) == 0) {
			temp[strlen(temp) - 1] = '\0';
			std::cout << "Client: " << temp << std::endl;
			break;
		}
		/*else {
			std::cout << "Message: ";
			getline(std::cin, str);
			c = str.c_str();
			send(NewSockid, c, sizeof(c), 0);
		}*/
		closesocket(sockid);
	}

		
		/*else {
			std::cout << "fail to connect to client !" << std::endl;
			closesocket(sockid);
		}*/
	system("PAUSE");
	return 0;
}
