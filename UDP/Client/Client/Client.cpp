// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048

int _tmain(int argc, _TCHAR* argv[])
{
	// Step 1: Inittiate Winsock
	WSADATA wsaData; // Win sock adress data
	WORD wVersion = MAKEWORD(2, 2); // 
	//  ? ?ây s? là giá tr? 0x0202 có ngh?a là phiên b?n 2.2
	if (WSAStartup(wVersion, &wsaData))
		printf(" Version is not supported .\n");
	printf("Client started! \n");

	// Step 2: Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//2,2,7
	// (optional) Set time-out for receiving
	int tv = 10000; //Time-out interval: 10000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	//Step 3: Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;// server address -> socket in network ( count of member =2 )
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	//Step 4: Communicate with server
	char buff[BUFF_SIZE];
	int ret, serverAddrLen = sizeof(serverAddr);
	do {
		// Send message
		printf("Send to server: ");
		gets_s(buff, BUFF_SIZE);
		ret = sendto(client, buff, strlen(buff), 0, (sockaddr *)&serverAddr, serverAddrLen); // Truyen gia tri vao dia tri serveraddr
		if (ret == SOCKET_ERROR)
			printf("Error! Cannot send message. ");

		// Receive echo message
		ret = recvfrom(client, buff, BUFF_SIZE, 0, (sockaddr *) &serverAddr, &serverAddrLen);

		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				printf("Time-out!");
			else printf("Error! Cannot receive message. ");
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Receive from server [%s:%d] %s \n", inet_ntoa(serverAddr.sin_addr), ntohs
				(serverAddr.sin_port), buff);
		}
		_strupr_s(buff, BUFF_SIZE);
	} while (strcmp(buff, "BYE") != 0); // End while

	// Step 5: Close socket
	closesocket(client);

	// Step 6 : Terminate Winsock
	WSACleanup();

    return 0;
}

