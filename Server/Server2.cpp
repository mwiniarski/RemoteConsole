//SERVER

#include <iostream>
#include <stdlib.h> //atoi
#include <errno.h>

using namespace std;
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <chrono>

#define CODE 932634
#define SER 0
#define WIN 1
#define CLI 2
#define MAX 3
#define BUFLEN 128


struct endpoint {
    struct sockaddr_in addr;
    int sock;
    bool isAvail;
};

struct endpoint endp[MAX];

char buf[BUFLEN];
socklen_t slen = sizeof(sockaddr_in);

void get_msg_from_client(int socket, struct sockaddr_in address){

    char temp_buf[BUFLEN];
    bzero(temp_buf, sizeof(temp_buf));

    // 5 * 200 ms = 1 sec
    for(int i = 0; i < 5; i++){

	//get one message - this is NON-blocking
	recv( socket, temp_buf, sizeof(temp_buf), MSG_DONTWAIT);

	//no message, w8 and try again
	if(temp_buf[0] == '\0') {
	    this_thread::sleep_for(chrono::milliseconds(200));
	    continue;
	}

	int code = atoi(temp_buf) % 1000000;
	int prefix = temp_buf[0] - '0';

	if(code == CODE && (prefix == WIN || prefix == CLI)) {
	    endp[prefix].sock = socket;
	    endp[prefix].addr = address;
	    endp[prefix].isAvail = true;
	}

	cout << code << " " << prefix << endl;
	//it got some message
	cout << "Confirmation msg: " << temp_buf << endl;
	return;
    }

    cout << "No confirmation msg\n";
    shutdown(socket, SHUT_RDWR);
}

void accept_incoming_connections(){
    int sock_new;
    struct sockaddr_in new_addr;

    while(true){

	//wait for connection
	sock_new = accept(endp[SER].sock, (struct sockaddr *) &new_addr, &slen);

	//connection is valid
	if(sock_new > 0){

	    //who is connectig?
	    cout << inet_ntoa(new_addr.sin_addr) << " port ";
	    cout << ntohs(new_addr.sin_port) << endl;

	    //strart new thread waiting for confirmation message
	    thread t2(get_msg_from_client, sock_new, new_addr);
	    t2.detach();
	    continue;
	}
	cout << "some error accepting!\n";
    }
}

int main(int argc, char* argv[]) {

    //can choose whichever port you want
    int PORT = 8888;
    if(argc == 2) {
	PORT = atoi(argv[1]);
    }

    //create a TCP socket
   if ((endp[SER].sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	cout << "Socket";
	return 0;
    }

    // zero out the structures
    for(int i=0; i < MAX; i++) {
	bzero((char *) &endp[i], sizeof(struct endpoint));
    }

    //set port && allow any incoming address
    endp[SER].addr.sin_family = AF_INET;
    endp[SER].addr.sin_port = htons(PORT);
    endp[SER].addr.sin_addr.s_addr = INADDR_ANY;

    //bind socket
    if( bind(endp[SER].sock , (struct sockaddr *) &endp[SER].addr, sizeof(endp[SER].addr)) < 0) {
		cout << "Bind";
		return 0;
    }

    //listen
    listen(endp[SER].sock, 5);
    cout << "WAITIN!\n";

    thread t1(accept_incoming_connections);
    t1.join();

    return 0;
}

