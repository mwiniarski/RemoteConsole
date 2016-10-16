#include "ClientConnection.h"
#include "IpDownloader.h"
#include <iostream>
#include <conio.h>
using namespace std;
#define BUFFSIZE 8096
void main(){
	LOG("CLIENT");

	//IpDownloader ipd;
	//cout << ipd.get();
	//Sleep(10000);
	//system("cls");
	ClientConnection con("89.72.12.25");
	con.start();

	char buffer[BUFFSIZE];
	
	for (;;){
		con.getMessage(buffer, sizeof(buffer));
		cout << buffer;
		ZeroMemory(buffer, sizeof(buffer));
		cin.getline(buffer, 128);
		con.sendMessage(buffer, strlen(buffer));
	}
}
