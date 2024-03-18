#pragma once

#include "KmBoxConfig.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <random>
#include <string>
#include <thread>


class KmBoxMouse
{
public:
	soft_mouse_t MouseData{};
public:
	// 移动
	int Move(int x, int y);
	// 自动轨迹移动
	int Move_Auto(int x, int y, int Runtime);
	// 左键
	int Left(bool Down);
	// 右键
	int Right(bool Down);
	// 中键
	int Middle(bool Down);
};

class KmBoxKeyBoard
{
public:
	std::thread t_Listen;
	WORD MonitorPort;
	SOCKET s_ListenSocket = 0;
	bool ListenerRuned = false;
public:
	standard_keyboard_report_t hw_Keyboard;
	standard_mouse_report_t hw_Mouse;
public:
	~KmBoxKeyBoard();
	void ListenThread();
	int StartMonitor(WORD Port);
	void EndMonitor();
public:
	bool GetKeyState(WORD vKey);
};

class KmBoxNetManager
{
private:
	SOCKADDR_IN AddrServer;
	SOCKET s_Client = 0;
	client_data ReceiveData;
	client_data PostData;
private:
	int NetHandler();
	int SendData(int DataLength);
public:
	~KmBoxNetManager();
	// 初始化设备
	int InitDevice(const std::string& IP, WORD Port, const std::string& Mac);
	// 重启设备
	int RebootDevice();
	// 设置设备s
	int SetConfig(const std::string& IP, WORD Port);
public:
	friend class KmBoxMouse;
	KmBoxMouse Mouse;
	friend class KmBoxKeyBoard;
	KmBoxKeyBoard KeyBoard;
};

inline KmBoxNetManager KmBoxMgr;
