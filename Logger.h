#include <iostream>
#include <string>
#include <conio.h>
//for colors
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//this only prints given string to STDOUT (so handful)
void LOG(std::string message, int number = -1, bool isFirst = false) {
	static int count = 0;

	if (isFirst){
		count = 0;
	}

	std::cout << "[" << count << "] ";
	if (count <= 9){
		std::cout << " ";
	}

	std::cout << message;

	if (number == 0) {
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << " OK";
		SetConsoleTextAttribute(hConsole, 7);
	}
	else if (number > 0) {
		std::cout << " " + std::to_string(number);
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << " ERROR";

		WSACleanup();
		_getch();
		exit(1);
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