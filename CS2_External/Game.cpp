#include "Game.h"

bool CGame::InitAddress()
{

	this->Address.ClientDLL = GetProcessModuleHandle(ProcessMgr.HANDLE, ProcessMgr.ProcessID,"client.dll");
	this->Address.MatchDLL = GetProcessModuleHandle(ProcessMgr.HANDLE, ProcessMgr.ProcessID, "matchmaking.dll");
	this->Address.EntityList = GetClientDLLAddress() + Offset::EntityList;
	this->Address.Matrix = GetClientDLLAddress() + Offset::Matrix;
	this->Address.ViewAngle = GetClientDLLAddress() + Offset::ViewAngle;
	this->Address.LocalController = GetClientDLLAddress() + Offset::LocalPlayerController;
	this->Address.LocalPawn = GetClientDLLAddress() + Offset::LocalPlayerPawn;
	this->Address.ForceJump = GetClientDLLAddress() + Offset::ForceJump;
	this->Address.GlobalVars = GetClientDLLAddress() + Offset::GlobalVars;

	return this->Address.ClientDLL != 0;
}

DWORD64 CGame::GetClientDLLAddress()
{
	return this->Address.ClientDLL;
}

DWORD64 CGame::GetMatchDLLAddress()
{
	return this->Address.MatchDLL;
}

DWORD64 CGame::GetEntityListAddress()
{
	return this->Address.EntityList;
}

DWORD64 CGame::GetMatrixAddress()
{
	return this->Address.Matrix;
}

DWORD64 CGame::GetViewAngleAddress() 
{
	return this->Address.ViewAngle;
}

DWORD64 CGame::GetEntityListEntry()
{
	return this->Address.EntityListEntry;
}

DWORD64 CGame::GetLocalControllerAddress()
{
	return this->Address.LocalController;
}

DWORD64 CGame::GetLocalPawnAddress()
{
	return this->Address.LocalPawn;
}

DWORD64 CGame::GetGlobalVarsAddress()
{
	return this->Address.GlobalVars;
}

bool CGame::UpdateEntityListEntry()
{
	DWORD64 EntityListEntry = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityListEntry))
		return false;
	if (!ProcessMgr.ReadMemory<DWORD64>(EntityListEntry + 0x10, EntityListEntry))
		return false;

	this->Address.EntityListEntry = EntityListEntry;

	return this->Address.EntityListEntry != 0;
}

bool CGame::SetViewAngle(float Yaw, float Pitch)
{
	Vec2 Angle{ Pitch,Yaw };

	if (!ProcessMgr.WriteMemory<Vec2>(this->Address.ViewAngle, Angle))
		return false;

	return true;
}

bool CGame::SetForceJump(int value)
{
	if (!ProcessMgr.WriteMemory<int>(this->Address.ForceJump, value))
		return false;

	return true;
}

uint64_t cbSize = 0x80000;
//callback for VfsFileListU
VOID cbAddFile(_Inout_ HANDLE h, _In_ LPSTR uszName, _In_ ULONG64 cb, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo)
{
	if (strcmp(uszName, "dtb.txt") == 0)
		cbSize = cb;
}
DWORD64 GetProcessModuleHandle(VMM_HANDLE HANDLE, DWORD ProcessID, std::string ModuleName)

{
	PVMMDLL_MAP_MODULEENTRY module_entry;
	bool result = VMMDLL_Map_GetModuleFromNameU(HANDLE, ProcessID, (LPSTR)ModuleName.c_str(), &module_entry, NULL);
	if (result) {
		return module_entry->vaBase;
	}
	if (!VMMDLL_InitializePlugins(HANDLE))
	{
		//std::cout << "[ERROR]: µ÷ÓÃVMMDLL_InitializePluginsÊ§°Ü" << std::endl;
		return 0;
	}

	//have to sleep a little or we try reading the file before the plugin initializes fully
	Sleep(1000);

	while (true)
	{
		BYTE bytes[4] = { 0 };
		DWORD i = 0;
		auto nt = VMMDLL_VfsReadW(HANDLE, (LPWSTR)L"\\misc\\procinfo\\progress_percent.txt", bytes, 3, &i, 0);
		if (nt == VMMDLL_STATUS_SUCCESS && atoi((LPSTR)bytes) == 100)
			break;

		Sleep(100);
	}

	VMMDLL_VFS_FILELIST2 VfsFileList;
	VfsFileList.dwVersion = VMMDLL_VFS_FILELIST_VERSION;
	VfsFileList.h = 0;
	VfsFileList.pfnAddDirectory = 0;
	VfsFileList.pfnAddFile = cbAddFile; //dumb af callback who made this system

	result = VMMDLL_VfsListU(HANDLE, (LPSTR)"\\misc\\procinfo\\", &VfsFileList);
	if (!result)
		return false;

	//read the data from the txt and parse it
	const size_t buffer_size = cbSize;
	std::unique_ptr<BYTE[]> bytes(new BYTE[buffer_size]);
	DWORD j = 0;
	auto nt = VMMDLL_VfsReadW(HANDLE, (LPWSTR)L"\\misc\\procinfo\\dtb.txt", bytes.get(), buffer_size - 1, &j, 0);
	if (nt != VMMDLL_STATUS_SUCCESS)
		return false;

	std::vector<uint64_t> possible_dtbs;
	std::string lines(reinterpret_cast<char*>(bytes.get()));
	std::istringstream iss(lines);
	std::string line;

	while (std::getline(iss, line))
	{
		Info info = { };

		std::istringstream info_ss(line);
		if (info_ss >> std::hex >> info.index >> std::dec >> info.process_id >> std::hex >> info.dtb >> info.kernelAddr >> info.name)
		{
			if (info.process_id == 0) //parts that lack a name or have a NULL pid are suspects
				possible_dtbs.push_back(info.dtb);
			if (ModuleName.find(info.name) != std::string::npos)
				possible_dtbs.push_back(info.dtb);
		}
	}

	//loop over possible dtbs and set the config to use it til we find the correct one
	for (size_t i = 0; i < possible_dtbs.size(); i++)
	{
		auto dtb = possible_dtbs[i];
		VMMDLL_ConfigSet(HANDLE, VMMDLL_OPT_PROCESS_DTB, dtb);
		result = VMMDLL_Map_GetModuleFromNameU(HANDLE, ProcessID, (LPSTR)ModuleName.c_str(), &module_entry, NULL);
		if (result)
		{
			return module_entry->vaBase;
		}
	}

	std::cout << "[-] Failed to patch module\n" << std::endl;
	return false;
}
