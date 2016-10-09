#include "Connection.h"
#include "RemoteConsole.h"
#include "IpUploadScheduler.h"


void main()
{
	IpUploadScheduler ipus(500);
	ipus.run();

	RemoteConsole rem;
	rem.start();
	
}