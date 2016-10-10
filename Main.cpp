#include "Connection.h"
#include "RemoteConsole.h"
#include "IpUploadScheduler.h"


void main()
{
	LOG("SERVER");
	//IpUploadScheduler ipus(500);
	//ipus.run();

	RemoteConsole rem;
	rem.start();
	
}