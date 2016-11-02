//SERVER

#include <iostream>
#include <ctime>
#include <stdlib.h> //atoi

using namespace std;
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define WINDOWS 5521859
#define CLIENT 3263094

int main(int argc, char* argv[]) {
    int PORT = 8888;
    if(argc == 2) {
		PORT = atoi(argv[1]);
    }

	struct sockaddr_in si_server, si_windows, si_client, si_unknown;


	int server_socket;
	unsigned appInfo, buf;
	socklen_t slen = sizeof(sockaddr_in), recv_len;

    //create a UDP socket
   if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
	cout << "Socket";
	return 0;
   }

    // zero out the structure
    memset(&si_server, 0, sizeof(si_server));
    memset(&si_windows, 0, sizeof(si_windows));
    memset(&si_client, 0, sizeof(si_client));
    memset(&si_unknown, 0, sizeof(si_unknown));

    //set port && any allow any incoming address
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket
    if( bind(server_socket , (struct sockaddr*)&si_server, sizeof(si_server) ) == -1) {

		cout << "Bind";
		return 0;
    }

    while (true) {
	//try to receive some data, this is a blocking call
	recv_len = recvfrom(server_socket, &buf, sizeof(buf), 0, (struct sockaddr *) &si_unknown, &slen);

	//interpret data
	if (buf == CLIENT) {
		si_client.sin_addr = si_unknown.sin_addr;
		si_client.sin_port = si_unknown.sin_port;
		cout << "CLIENT endpoint: " << inet_ntoa(si_client.sin_addr) << ntohs(si_client.sin_port) << endl; }
	else if (buf == WINDOWS) {
		si_windows.sin_addr = si_unknown.sin_addr;
		si_windows.sin_port = si_unknown.sin_port;
		cout << "WINDOWS endpoint: " << inet_ntoa(si_client.sin_addr) <<  ntohs(si_client.sin_port) << endl;
	}
	else {
		cout << "Unknown packet" << endl;
	}
    }

    //send some!
    while (true)
    {
            cin >> buf;

       //now reply the client with the same data
       if (sendto(server_socket, &buf, sizeof(buf), 0, (struct sockaddr*) &si_unknown, slen) == -1)
       {
            cout << "sendto()";
	    exit(1);
       }
    }

    return 0;
}
