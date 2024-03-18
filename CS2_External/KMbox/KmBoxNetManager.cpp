#include "ExcludedH.h"

#include "KmBoxNetManager.h"



KmBoxNetManager::~KmBoxNetManager()
{
	WSACleanup();
	if (this->s_Client != NULL)
	{
		closesocket(this->s_Client);
		this->s_Client = NULL;
	}
}

int KmBoxNetManager::InitDevice(const std::string& IP, WORD Port, const std::string& Mac)
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;	
	int Status = 0;

	Status = WSAStartup(wVersionRequested, &wsaData);
	if (Status != success)
		return err_creat_socket;
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		this->s_Client = -1;
		return err_net_version;
	}
	srand(time(NULL));

	this->s_Client = socket(AF_INET, SOCK_DGRAM, 0);

	this->AddrServer.sin_addr.S_un.S_addr = inet_addr(IP.c_str());
	this->AddrServer.sin_family = AF_INET;
	this->AddrServer.sin_port = htons(Port);

	this->PostData.head.mac = std::stoll(Mac, nullptr, 16);
	this->PostData.head.rand = rand();
	this->PostData.head.indexpts = 0;
	this->PostData.head.cmd = cmd_connect;

	Status = sendto(this->s_Client, reinterpret_cast<const char*>(&this->PostData), sizeof(cmd_head_t), 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), sizeof(this->AddrServer));
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	int FromLen = sizeof(this->AddrServer);

	Status = recvfrom(this->s_Client, reinterpret_cast<char*>(&this->ReceiveData), 1024, 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), &FromLen);
	if (Status < 0)
		return err_net_rx_timeout;

	return NetHandler();
}

int KmBoxNetManager::SendData(int DataLength)
{
	int Status;
	sendto(this->s_Client, reinterpret_cast<const char*>(&this->PostData), DataLength, 0,
		reinterpret_cast<sockaddr*>(&this->AddrServer), sizeof(this->AddrServer));

	SOCKADDR_IN s_NewClient;
	int FromLen = sizeof(s_NewClient);

	Status = recvfrom(this->s_Client, reinterpret_cast<char*>(&this->ReceiveData), 1024, 0,
		reinterpret_cast<sockaddr*>(&s_NewClient), &FromLen);

	if (Status < 0)
		return err_net_rx_timeout;

	return NetHandler();
}

int KmBoxNetManager::RebootDevice()
{
	if (this->s_Client <= 0)
		return err_creat_socket;
	
	int Status = 0;

	this->PostData.head.indexpts++;
	this->PostData.head.cmd = cmd_reboot;
	this->PostData.head.rand = rand();

	int Length = sizeof(cmd_head_t);

	this->SendData(Length);
	
	WSACleanup();
	this->s_Client = -1;

	if (Status < 0)
		return err_net_rx_timeout;
	
	return NetHandler();
}

int KmBoxNetManager::SetConfig(const std::string& IP, WORD Port)
{
	if (this->s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	this->PostData.head.indexpts++;
	this->PostData.head.cmd = cmd_setconfig;
	this->PostData.head.rand = inet_addr(IP.c_str());
	this->PostData.u8buff.buff[0] = Port >> 8;
	this->PostData.u8buff.buff[1] = Port >> 0;

	int Length = sizeof(cmd_head_t) + 2;

	return this->SendData(Length);
}

int KmBoxNetManager::NetHandler()
{
	if (ReceiveData.head.cmd != PostData.head.cmd)
		return  err_net_cmd;
	if (ReceiveData.head.indexpts != PostData.head.indexpts)
		return  err_net_pts;
	return 0;
}

int KmBoxMouse::Move(int x, int y)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_move;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.x = x;
	this->MouseData.y = y;

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	this->MouseData.x = 0;
	this->MouseData.y = 0;

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Move_Auto(int x, int y, int Runtime)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_automove;
	KmBoxMgr.PostData.head.rand = Runtime;

	this->MouseData.x = x;
	this->MouseData.y = y;

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	this->MouseData.x = 0;
	this->MouseData.y = 0;

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Left(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_left;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x01) : (this->MouseData.button & (~0x01)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Right(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_right;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x02) : (this->MouseData.button & (~0x02)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}

int KmBoxMouse::Middle(bool Down)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_mouse_middle;
	KmBoxMgr.PostData.head.rand = rand();

	this->MouseData.button = (Down ? (this->MouseData.button | 0x04) : (this->MouseData.button & (~0x04)));

	memcpy_s(&KmBoxMgr.PostData.cmd_mouse, sizeof(soft_mouse_t), &this->MouseData, sizeof(soft_mouse_t));

	int Length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);

	return KmBoxMgr.SendData(Length);
}

void KmBoxKeyBoard::ListenThread()
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int Status = 0;

	Status = WSAStartup(wVersionRequested, &wsaData);
	if (Status != success)
		return;

	this->s_ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in AddrServer{};

	AddrServer.sin_family = PF_INET;
	AddrServer.sin_addr.S_un.S_addr = INADDR_ANY;
	AddrServer.sin_port = htons(this->MonitorPort);

	Status = bind(this->s_ListenSocket, reinterpret_cast<SOCKADDR*>(&AddrServer), sizeof(SOCKADDR));

	SOCKADDR AddrClient{};

	int FromLen = sizeof(SOCKADDR);

	char Buffer[1024]{};

	this->ListenerRuned = true;
	while (this->ListenerRuned)
	{
		int Ret = recvfrom(this->s_ListenSocket, Buffer, 1024, 0, &AddrClient, &FromLen);
		if (Ret > 0)
		{
			memcpy(&this->hw_Mouse, Buffer, sizeof(this->hw_Mouse));
			memcpy(&this->hw_Keyboard, &Buffer[sizeof(this->hw_Mouse)], sizeof(this->hw_Keyboard));
		}
		else
			break;
	}
	this->ListenerRuned = false;
	this->s_ListenSocket = 0;
}

int KmBoxKeyBoard::StartMonitor(WORD Port)
{
	if (KmBoxMgr.s_Client <= 0)
		return err_creat_socket;

	int Status = 0;

	KmBoxMgr.PostData.head.indexpts++;
	KmBoxMgr.PostData.head.cmd = cmd_monitor;

	this->MonitorPort = Port;
	KmBoxMgr.PostData.head.rand = Port | 0xaa55 << 16;

	int Length = sizeof(cmd_head_t);

	KmBoxMgr.SendData(Length);

	if (this->s_ListenSocket > 0)
	{
		closesocket(this->s_ListenSocket);
		this->s_ListenSocket = 0;
	}
	
	this->t_Listen = std::thread(&KmBoxKeyBoard::ListenThread, this);

	std::this_thread::sleep_for(std::chrono::milliseconds(8));

	return KmBoxMgr.NetHandler();
}

void KmBoxKeyBoard::EndMonitor()
{
	if (this->ListenerRuned)
	{
		this->ListenerRuned = false;

		if(this->s_ListenSocket)
			closesocket(this->s_ListenSocket);

		this->s_ListenSocket = 0;
		this->MonitorPort = 0;
		this->t_Listen.join();
	}
}

KmBoxKeyBoard::~KmBoxKeyBoard()
{
	this->EndMonitor();
}

bool KmBoxKeyBoard::GetKeyState(WORD vKey)
{
	unsigned char KeyValue = vKey & 0xff;
	if (!this->ListenerRuned)
		return false;

	if (KeyValue >= KEY_LEFTCONTROL && KeyValue <= KEY_RIGHT_GUI)
	{
		switch (KeyValue)
		{
		case KEY_LEFTCONTROL: return  this->hw_Keyboard.buttons & BIT0 ? 1 : 0;
		case KEY_LEFTSHIFT:   return  this->hw_Keyboard.buttons & BIT1 ? 1 : 0;
		case KEY_LEFTALT:     return  this->hw_Keyboard.buttons & BIT2 ? 1 : 0;
		case KEY_LEFT_GUI:    return  this->hw_Keyboard.buttons & BIT3 ? 1 : 0;
		case KEY_RIGHTCONTROL:return  this->hw_Keyboard.buttons & BIT4 ? 1 : 0;
		case KEY_RIGHTSHIFT:  return  this->hw_Keyboard.buttons & BIT5 ? 1 : 0;
		case KEY_RIGHTALT:    return  this->hw_Keyboard.buttons & BIT6 ? 1 : 0;
		case KEY_RIGHT_GUI:   return  this->hw_Keyboard.buttons & BIT7 ? 1 : 0;
		default:			  return false;
		}
	}
	else
	{
		for (auto i : this->hw_Keyboard.data)
		{
			if (i == KeyValue)
				return true;
		}
	}
	return false;
}