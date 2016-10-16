#include <ws2tcpip.h>
#include <stdlib.h>
#include "../Logger.h"
#pragma comment (lib, "Ws2_32.lib")


#define PORT "59289"

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
		LOG("WSAStartup", iResult);
		
		// set address information
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
		hints.ai_flags = AI_PASSIVE;

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

	int start(){
		// if client waiting, accept the connection and save the socket
		LOG("Waiting for client...");
		ClientSocket = accept(ListenSocket, NULL, NULL);
		iResult = 0;

		if (ClientSocket == INVALID_SOCKET) {
			iResult = WSAGetLastError();
			return 0;
		}

		LOG("Client accepted","Client accept", iResult);	
		LOG("Messages from client: ");
		return 1;
	}

	//send message to client
	void sendMessage(char * message, int length){
		send(ClientSocket, message, length + 1, 0);
	}

	//get message from client - blocking!
	int getMessage(char * message, int size){
		iResult = recv(ClientSocket, message, size, 0);

		//happens when client exits application or connation fails
		if (iResult == SOCKET_ERROR){
			LOG("|Connection closed", -2);
			return 0;
		}
		return 1;
	}

	~Connection(){
		// shutdown the connection since we're done
		shutdown(ClientSocket, SD_SEND);
	
		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
	}
};