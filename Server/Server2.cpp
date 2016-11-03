//SERVER

#include <iostream>
#include <ctime>
#include <stdlib.h> //atoi
#include <unistd.h>
#include <errno.h>
using namespace std;
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define WINDOWS 5521859
#define CLIENT 3263094

int main(int argc, char* argv[]) {

    int PORT = 8888;
    if(argc == 2) {
		PORT = atoi(argv[1]);
    }
    struct sockaddr_in server_addr, client_addr;
    int sock_server, sock_new;

    unsigned appInfo;
    char buf[128];
    socklen_t slen = sizeof(sockaddr_in);

    //create a TCP socket
   if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	cout << "Socket";
	return 0;
   }

    // zero out the structure
    bzero((char *)&server_addr, sizeof(server_addr));

    //set port && allow any incoming address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //bind socket
    if( bind(sock_server , (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0) {

		cout << "Bind";
		return 0;
    }

    listen(sock_server, 5);
    cout << "WAITIN!\n";
    if(( sock_new = accept(sock_server, NULL, NULL)) < 0){
	cout << "accept";
	return 0;
    }

    int a;
    bzero(buf,sizeof(buf));
    cout << "IM HERE :)))\n";
    while ((a = recv(sock_new, buf, sizeof(buf), 0)) > 0){
	cout << "Buffer: " << buf << endl;
	bzero(buf,sizeof(buf));
    }
    cout << a << strerror(errno) << endl;

    return 0;
}

