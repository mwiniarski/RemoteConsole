#include "Connection.h"
#include "RemoteConsole.h"
#include "IpUploadScheduler.h"

void mainLoop(){

	LOG("SERVER");

	Connection * theConnection = new Connection();
	RemoteConsole * console;
	for (;;) {
		console = new RemoteConsole(theConnection);
		console->start();
		delete console;
	}
}



void main()
{
	IpUploadScheduler ipus(500);
	ipus.run();
	mainLoop();


}