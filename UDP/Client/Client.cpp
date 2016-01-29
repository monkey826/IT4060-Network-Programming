// UDPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "winsock2.h"
#include "ws2tcpip.h"

#define BUFF_SIZE 2048

int main(int argc,char* argv[])
{
	//Step 1: Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		printf("Version is not supported.\n");

	printf("Client started!\n");

	//Step 2: Construct socket	
	SOCKET client;
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//(optional) Set time-out for receiving
	int tv = 10000; //Time-out interval: 10000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	//Step 3: Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);

	//Step 4: Communicate with server
	char buff[BUFF_SIZE];
	int ret, serverAddrLen = sizeof(serverAddr);
	do {
		//Send message
		printf("Send to server: ");
		gets_s(buff, BUFF_SIZE);
		ret = sendto(client, buff, strlen(buff), 0, (sockaddr *)&serverAddr, serverAddrLen);
		if (ret == SOCKET_ERROR)
			printf("Error! Cannot send mesage.");

		//Receive echo message
		ret = recvfrom(client, buff, BUFF_SIZE, 0, (sockaddr *)&serverAddr, &serverAddrLen);

		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				printf("Time-out!");
			else printf("Error! Cannot receive message.");
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Receive from server[%s:%d] %s\n",
				inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), buff);
		}
		_strupr_s(buff, BUFF_SIZE);
	} while (strcmp(buff, "BYE") != 0); //end while

										//Step 5: Close socket
	closesocket(client);

	//Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}

