#include "KmboxB.h"

#include "kmbox_b.hpp"



int KmBoxBManager::init()
{
	std::string port = find_port("USB-SERIAL CH340"); // name of the kmbox port without ( COM )
	if (port.empty()) {
		return -1;
	}
	if (!open_port(hSerial, port.c_str(), CBR_115200)) { // CBR_1115200 is the baud rate
		return -2;
	}
	return 0;
}

void KmBoxBManager::km_move(int X, int Y)
{
	std::string command = "km.move(" + std::to_string(X) + "," + std::to_string(Y) + ")\r\n";
	send_command(hSerial, command.c_str());
}

void KmBoxBManager::km_move_auto(int X, int Y, int runtime)
{
	std::string command = "km.move(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(runtime) + ")\r\n";
	send_command(hSerial, command.c_str());
}

void KmBoxBManager::km_click()
{
	std::string command = "km.left(" + std::to_string(1) + ")\r\n"; // left mouse button down
	Sleep(10); // to stop it from crashing idk
	std::string command1 = "km.left(" + std::to_string(0) + ")\r\n"; // left mouse button up
	send_command(hSerial, command.c_str());
	send_command(hSerial, command1.c_str());
}
