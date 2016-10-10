#include <ws2tcpip.h>
#include <stdlib.h>
#include "Logger.h"
#pragma comment (lib, "Ws2_32.lib")

#define PORT "38010"
#define PORT2 38010

class Connection{
private:
	//THINGS
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

public:
	Connection(){

		// create WSADATA object
		WSADATA wsaData;
		
		// our sockets for the server
		ListenSocket = INVALID_SOCKET;
		ClientSocket = INVALID_SOCKET;

		// address info for the server to listen to
		struct addrinfo *result = NULL;
		struct addrinfo hints;
		
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		LOG("WSAStartup", iResult, true);
		
		// set address information
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
		hints.ai_flags = AI_PASSIVE;

		//////////////////////
/*		sockaddr_in localAddress;
		localAddress.sinfamily = AF_INET;
		localAddress.sin_port = htons(10000);  // or whatever port you'd like to listen to
		localAddress.sin_addr.s_addr = INADDR_ANY;

		bind(s, (SOCKADDR*)&localAddress, sizeof(localAddress));
		listen(s, SOMAXCONN);
	*/	/////////////////////////

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, PORT, &hints, &result);
		LOG("Get address info", iResult);

		// Create a SOCKET for connecting to server
		ListenSocket = WSASocket(result->ai_family, result->ai_socktype, result->ai_protocol, NULL, 0, WSA_FLAG_NO_HANDLE_INHERIT);
		if (ListenSocket == INVALID_SOCKET) {
			freeaddrinfo(result);
			iResult = WSAGetLastError();
		}
		LOG("Creating socket", iResult);

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			freeaddrinfo(result);
			closesocket(ListenSocket);
		}
		LOG("Binding", iResult);

		// no longer need address information
		freeaddrinfo(result);

		// start listening for new clients attempting to connect
		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			closesocket(ListenSocket);
		}
		LOG("Listening", iResult);
	}

	void start(){
		// if client waiting, accept the connection and save the socket
		LOG("Waiting for client...");
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			iResult = WSAGetLastError();
			return;
		}
		LOG("Client accepted","Socket error", iResult);
	
		// No longer need server socket
		closesocket(ListenSocket);
	}

	void sendMessage(char * message, int length){
		send(ClientSocket, message, length + 1, 0);
	}
	void getMessage(char * message, int size){
		recv(ClientSocket, message, size, 0);
	}

	~Connection(){
		//freeaddrinfo(iResult);
		// shutdown the connection since we're done
		shutdown(ClientSocket, SD_SEND);

		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
	}
};