#include <iostream>
#include <string>
#include <conio.h>

#define GREEN 2
#define RED 4
#define YELLOW 6
#define WHITE 7
#define GREY 8
//for colors
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//this only prints given string to STDOUT (so handful)
void LOG(std::string message, int number = -1) {
	
	static int count = 0;

	if (message[0] == '|') {
		std::cout << std::endl;
		message = message.substr(1);
	}
	if (number == -3) {
		SetConsoleTextAttribute(hConsole, GREY);
		std::cout << message + ", ";
		return;
	}

	//set initially to white
	SetConsoleTextAttribute(hConsole, WHITE);

	//keep alerts in nice, clean order
	std::cout << "[" << count << "] ";
	if (count <= 9){
		std::cout << " ";
	}

	//to sygnalize something
	if (number == -2) {
		SetConsoleTextAttribute(hConsole, YELLOW);
		std::cout << message;
	}
	//everything good
	else if (number == 0) {
		std::cout << message;
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << " OK";
	}
	//errors - end of execution
	else if (number > 0) {
		std::cout << message + " " + std::to_string(number);
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << " ERROR";

		WSACleanup();
		_getch();
		exit(1);
	}
	else {
		std::cout << message;
	}

	std::cout << std::endl;
	count++;
}

void LOG(std::string goodMessage, std::string badMessage, int number) {
	if (number == 0) {
		LOG(goodMessage, 0);
	}
	else {
		LOG(badMessage, number);
	}
}