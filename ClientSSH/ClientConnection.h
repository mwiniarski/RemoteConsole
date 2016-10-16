// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h> 
#include <string>
#include "../Logger.h"
using namespace std;

// port to connect sockets through 
#define DEFAULT_PORT "59289"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientConnection
{
private:
	// for error checking function calls in Winsock library
	int iResult;

	// socket for client to connect to server
	SOCKET ConnectSocket;

	//result for connection
	struct addrinfo *result = NULL;
public:
	ClientConnection(PCSTR hostname){

		WSADATA wsaData;
		ConnectSocket = INVALID_SOCKET;

		// holds address info for socket to connect to
		struct addrinfo hints;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		LOG("WSAStartup", iResult);

		// set address info
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

		//resolve server address and port
		
		iResult = getaddrinfo(hostname, DEFAULT_PORT, &hints, &result);
		LOG("Get address info", iResult);
	}
	void start(){
			
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			iResult = WSAGetLastError();
		}
		LOG("Creating socket", iResult);

		// Connect to server.
		iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}
		LOG("Connected", "Server is down... did not connect", iResult);

		// no longer need address info for server
		freeaddrinfo(result);
	}

	void sendMessage(char * message, int length){
		//you always pass lenght as strlen(buffer) so +1 is for \0
		send(ConnectSocket, message, length + 1, 0);
	}
	void getMessage(char * message, int size){
		//1 byte of space for \0
		recv(ConnectSocket, message, size - 1, 0);
	}
};