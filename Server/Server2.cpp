//SERVER

#include <iostream>
#include <ctime>
using namespace std;

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
	#include<ws2tcpip.h>
#else 
	#include<sys/socket.h>
	#include<arpa/inet.h>
#endif

#define WINDOWS 5521859;
#define CLIENT  3263094;

int main(int argc, char* argv[])
{
    int PORT = 8888;
    if(argc == 2) {
		PORT = atoi(argv[1]);
    }

	struct sockaddr_in si_server, si_windows, si_client, si_unknown;


	int socket;
	unsigned appInfo;
	socklen_t slen = sizeof(sockaddr_in), recv_len;

    //create a UDP socket
	if ((socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		cout << "Socket";
		return 0;
	}

	// zero out the structure
    memset((char *)&si_server, 0, sizeof(si_server));
	memset((char *)&si_windows, 0, sizeof(si_windows));
	memset((char *)&si_client, 0, sizeof(si_client));
	memset((char *)&si_unknown, 0, sizeof(si_unknown));

	//set port && any allow any incoming address
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket
    if( bind(socket , (struct sockaddr*)&si_server, sizeof(si_server) ) == -1)
    {
		cout << "Bind";
		return 0;
    }

	while (true) {
		//try to receive some data, this is a blocking call
		recv_len = recvfrom(socket, &buf, sizeof(buf), 0, (struct sockaddr *) &si_unknown, &slen);
		
		//interpret data
		if (buf == CLIENT) {
			si_client.sin_addr = si_unknown.sin_addr;
			si_client.sin_port = si_unknown.sin_port;
			cout << "CLIENT endpoint: " << inet_ntoa(si_client.sin_addr) << ntohs(si_client.sin_port) << endl;
		}
		else if (buf == WINDOWS) {
			si_windows.sin_addr = si_unknown.sin_addr;
			si_windows.sin_port = si_unknown.sin_port;
			cout << "WINDOWS endpoint: " << inet_ntoa(si_client.sin_addr) << ntohs(si_client.sin_port) << endl;
		}
		else {
			cout << "Unknown packet" << endl;
		}
	}

    //send some!
    while(1)
    {
        scanf("%s", buf);

       //now reply the client with the same data
       if (sendto(socket, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen) == -1)
       {
            printf("sendto()");
	    exit(1);
       }
    }

    return 0;
}
