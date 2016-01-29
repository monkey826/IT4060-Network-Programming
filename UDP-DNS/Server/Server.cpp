
// Author : monkey
#include "stdafx.h"
#include "conio.h"
#include "stdio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define BUFF_SIZE 2048
int main(int argc, char* argv[]) {
	// Step 1: Initiate winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		printf(" Version is not supported \n ");
	// Step 2: Construct socket
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	// Step 3: bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);

	if (bind(server, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		printf(" Error! Cannot bind this address.");
		getch();
		return 0;
	}
	printf(" Server Started! \n ");
	// Step 4: communicate with client
	sockaddr_in clientAddr, resultAddr;
	char buff[BUFF_SIZE], msg[BUFF_SIZE];
	int ret, clientAddrLen = sizeof(clientAddr);
	addrinfo *pHead = NULL; // using pointer type of struct addrinfo
	while (1) {
		// Receive message
		ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
		if (ret == SOCKET_ERROR)
			printf(" Error: %d", WSAGetLastError());
		else if (ret > 0)
		{
			buff[ret] = 0;
			printf(" Receive from client [%s:%d] %s \n",
				inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port), buff);
			// Get IP address from domain
			ret = getaddrinfo(buff, "http", NULL, &pHead);//Get name -> IP -> pHead pointer
			if (ret != 0) {
				sprintf(msg, "Server: Resolve failed %d ! \n", ret);
				// echo to client
				sendto(server, msg, strlen(msg), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
			}
			else {
				while (pHead != NULL) { // Loop pointer
					memcpy(&resultAddr, pHead->ai_addr, pHead->ai_addrlen); // Cpy addr -> resultAddr.
					sprintf(msg, "Server: %s:%d \n", inet_ntoa(resultAddr.sin_addr), htons(resultAddr.sin_port));
					 //echo to client 
					sendto(server, msg, strlen(msg), 0, (sockaddr *)&clientAddr,
						sizeof(clientAddr));
					pHead = pHead->ai_next;// Pointer next for next domain
				} // end while
			}
		}
	}// end while
	 // Step 5: Close socket
	closesocket(server);
	// Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}