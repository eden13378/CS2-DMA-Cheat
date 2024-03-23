#include "KMbox/KmboxB.h"
#include "KMbox/KmBoxNetManager.h"
#include "KmBoxData.h"
#include "Utils/Format.hpp"
#include "CheatsThread.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <windows.h>
#include <filesystem>

namespace fs = std::filesystem;

#pragma comment(lib, "D3DX11.lib")

int main()
{
	std::cout << "[ info ] DMA Software for cs2 // https://github.com/IvanAcoola" << std::endl;
	Offset::UpdateOffsets();
	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");

	if (ProcessStatus != StatusCode::SUCCEED)
	{
		std::cout << "[ error ] Failed to attach process, StatusCode:" << ProcessStatus << std::endl;
		return -1;
	}

	ProcessMgr.init_keystates();

	if (!gGame.InitAddress())
	{
		std::cout << "[ error ] Failed to call InitAddress()." << std::endl;
		return -1;
	}

	if (!fs::exists("offsets.json") || !fs::exists("client.dll.json"))
	{
		std::cout << "[ error ] client.dll.json or offsets.json not found!" << std::endl;
		return -1;
	}

	if (!fs::exists("kmbox.json"))
	{
		std::cout << "[ error ] ckmbox.json not found!" << std::endl;
		return -1;
	}

	KmBoxDataGetter kmbx("kmbox.json");

	if (kmbx.type == "net") {
		if (KmBoxMgr.InitDevice(kmbx.ip, kmbx.port, kmbx.uuid) != 0)
		{
			std::cout << "[ error ] KmBoxNet Initialize failed." << std::endl;
			return -1;
		}
		KmBox::type = "net";
	}
	else if (kmbx.type == "b") {
		if (kmBoxBMgr.init() != 0) {
			std::cout << "[ error ] KmBoxB Initialize failed." << std::endl;
			return -1;
		}
		KmBox::type = "b";
	}

	if (!fs::directory_entry(MenuConfig::path).exists()) {
		fs::create_directory(MenuConfig::path);
	}

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateMatrix), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(LoadLocalEntity), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(LoadEntity), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateEntityListEntry), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(ScatterReadThreads), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateWeaponNameThreads), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(KeysCheckThread), NULL, 0, 0);

	Gui.NewWindow("CS2DMA", Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), Cheats::Run);

	return 0;
}