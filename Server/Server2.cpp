/*
    Simple udp server
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512  //Max length of buffer
//#define PORT 8888   //The port on which to listen for incoming data

int main(int argc, char* argv[])
{
    int PORT = 8888;
    if(argc == 2) {
	PORT = atoi(argv[1]);
    }

    struct sockaddr_in si_me, si_other;

    int s, i;
    socklen_t slen = sizeof(si_other), recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("socket");
	exit(1);
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        printf("bind");
	exit(1);
    }

    //try to receive some data, this is a blocking call
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
    {
        printf("recvfrom()");
	exit(1);
    }
    printf("Client endpoint:  %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

    //send some!
    while(1)
    {
        scanf("%s", buf);

       //now reply the client with the same data
       if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen) == -1)
       {
            printf("sendto()");
	    exit(1);
       }
    }

    return 0;
}
