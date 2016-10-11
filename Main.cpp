#include "Connection.h"
#include "RemoteConsole.h"
#include "IpUploadScheduler.h"


void main()
{
	LOG("SERVER");
	//IpUploadScheduler ipus(500);
	//ipus.run();

	Connection * theConnection = new Connection();
	RemoteConsole * console;
	for (;;) {
		console = new RemoteConsole(theConnection);
		console->start();
		delete console;
	}
	
}