#pragma once

#include <Windows.h>
#include <string>

class KmBoxBManager {
private:
	HANDLE hSerial;
public:
	int init();

	void km_move(int X, int Y);

	void km_move_auto(int X, int Y, int runtime);

	void km_click();

};

inline KmBoxBManager kmBoxBMgr;