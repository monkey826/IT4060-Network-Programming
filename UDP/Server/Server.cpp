// UDPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define BUFF_SIZE 2048

int main(int argc, char* argv[])
{
	//Step 1: Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		printf("Version is not supported\n");

	//Step 2: Construct socket	
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);
	if (bind(server, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error! Cannot bind this address.");
		getch();
		return 0;
	}
	printf("Server started! \n");

	//Step 4: Communicate with client
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE];
	int ret, clientAddrLen = sizeof(clientAddr);
	int sum = 0;
	while (1) {
		//Receive message
		ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
		if (ret == SOCKET_ERROR)
			printf("Error : %d", WSAGetLastError());
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Receive from client[%s:%d] %s\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
			sum += ret;
			printf(" Receive:  %d Byte \n", sum);
			//Echo to client
			ret = sendto(server, buff, strlen(buff), 0, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
			if (ret == SOCKET_ERROR)
				printf("Error: %d", WSAGetLastError());
		}
	} //end while

	  //Step 5: Close socket
	closesocket(server);

	//Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}

