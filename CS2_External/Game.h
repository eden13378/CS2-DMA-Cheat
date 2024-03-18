#pragma once
#include <optional>
#include "Utils/ProcessManager.hpp"
#include "Offsets.h"
#include "View.hpp"
#include<iostream>
#include<fstream>
#include<sstream>
class CGame
{
private:
	struct
	{
		DWORD64 ClientDLL;
		DWORD64 MatchDLL;
		DWORD64 EntityList;
		DWORD64 Matrix;
		DWORD64 ViewAngle;
		DWORD64 EntityListEntry;
		DWORD64 LocalController;
		DWORD64 LocalPawn;
		DWORD64 ForceJump;
		DWORD64 GlobalVars;
	}Address;

public:
	CView View;

public:	

	bool InitAddress();

	DWORD64 GetClientDLLAddress();

	DWORD64 GetMatchDLLAddress();

	DWORD64 GetEntityListAddress();

	DWORD64 GetMatrixAddress();

	DWORD64 GetViewAngleAddress();

	DWORD64 GetEntityListEntry();

	DWORD64 GetLocalControllerAddress();

	DWORD64 GetLocalPawnAddress();

	DWORD64 GetGlobalVarsAddress();

	bool UpdateEntityListEntry();

	bool SetViewAngle(float Yaw, float Pitch);

	bool SetForceJump(int Value);
};

inline CGame gGame;

DWORD64 GetProcessModuleHandle(VMM_HANDLE HANDLE, DWORD  ProcessID, std::string ModuleName);


