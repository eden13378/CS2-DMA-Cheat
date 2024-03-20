#include "CheatsThread.h"
#include <synchapi.h>
using namespace Cheats;



VOID UpdateMatrix()
{
	while (true)
	{
		Sleep(1);
		// Update matrix
		if (!ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix, 64))
			return;
	}
}

VOID LoadLocalEntity()
{
	while (true)
	{
		Sleep(1);
		// LocalEntity
		DWORD64 LocalControllerAddress = 0;
		DWORD64 LocalPawnAddress = 0;
		if (!ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress))
			continue;
		if (!ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress))
			continue;
		CEntity LocalPlayer;
		if (!LocalPlayer.UpdateController(LocalControllerAddress))
			continue;
		if (!LocalPlayer.UpdatePawn(LocalPawnAddress))
			continue;

		LocalEntityPlayer = LocalPlayer;
	}
}
std::vector<CEntity> TempEntityList;

bool isSameCEntity(std::vector<CEntity>list1, std::vector<CEntity>list2) {
	bool isSame = true;
	if (list1.size() != list2.size()) {
		isSame = false;
	}
	return isSame;
}
VOID UpdatePlayer(int index) {
	CEntity Entity;
	DWORD64 EntityAddress = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (index + 1) * 0x78, EntityAddress))
		return;
	if (EntityAddress == LocalEntityPlayer.Controller.Address)
	{
		LocalEntityPlayer.LocalPlayerControllerIndex = index;
		return;
	}

	if (!Entity.UpdateController(EntityAddress))
		return;

	//if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID)
	//	return;

	if (!Entity.UpdatePawn(Entity.Pawn.Address))
		return;

	if (!Entity.IsAlive())
		return;

	TempEntityList.push_back(Entity);


}
VOID LoadEntity()
{
	while (true)
	{
		if (LocalEntityPlayer.Controller.Address == 0) {
			continue;
		}
		std::vector<std::thread> threads;
		TempEntityList.clear();
		for (int i = 0; i < 64; i++)
		{
			threads.push_back(std::thread(UpdatePlayer, i));
		}
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		if (!isSameCEntity(EntityList, TempEntityList)) {
			EntityList = TempEntityList;
		}

		Sleep(1);
	}
}

VOID UpdateEntityListEntry()
{
	while (true)
	{
		// Update EntityList Entry
		gGame.UpdateEntityListEntry();

		uintptr_t mapaddress;
		ProcessMgr.ReadMemory(gGame.GetMatchDLLAddress() + 0x001D2300, mapaddress);
		ProcessMgr.ReadMemory(mapaddress + 0x4, mapname);

		Sleep(5000);
	}
}

VOID UpdateWeaponName(int index) {
	EntityList[index].Pawn.GetWeaponName();
}

VOID UpdateVlue(int index) {

	std::vector<BoneJointPos> BonePosList;
	for (int i = 0; i < 30; i++)
	{
		Vec2 ScreenPos;
		bool IsVisible = false;

		if (gGame.View.WorldToScreen(EntityList[index].TempBoneArray[i].Pos, ScreenPos))
			IsVisible = true;

		BonePosList.push_back({ EntityList[index].TempBoneArray[i].Pos ,ScreenPos,IsVisible });
	}
	EntityList[index].Pawn.BoneData.BonePosList = BonePosList;

	EntityList[index].Pawn.Pos = EntityList[index].TempPos;
	EntityList[index].Pawn.Health = EntityList[index].TempHealth;
	EntityList[index].Pawn.ViewAngle = EntityList[index].TempViewAngle;
	EntityList[index].Pawn.bSpottedByMask = EntityList[index].TempbSpottedByMask;
}

VOID ScatterReadThreads()
{
	while (true)
	{
		Sleep(1);
		clock_t start = clock();
		VMMDLL_SCATTER_HANDLE handle = ProcessMgr.CreateScatterHandle();

		for (int i = 0; i < EntityList.size(); i++)
		{
			//¶Á¹Ç÷À

			ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.BoneData.BoneArrayAddress, EntityList[i].TempBoneArray, 30 * sizeof(BoneJointData));
			//¶ÁÈ¡Î»ÖÃ
			ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::Pos, &EntityList[i].TempPos, sizeof(Vec3));
			//¶ÁÈ¡ÑªÁ¿
			ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::CurrentHealth, &EntityList[i].TempHealth, sizeof(int));
			//¶ÁÈ¡ÊÓÒ°
			ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::angEyeAngles, &EntityList[i].TempViewAngle, sizeof(Vec2));
			//ÑÚÌå¼ì²â
			ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::bSpottedByMask, &EntityList[i].TempbSpottedByMask, sizeof(DWORD64));
		}
		ProcessMgr.ExecuteReadScatter(handle);


		std::vector<std::thread> threads;
		for (int i = 0; i < EntityList.size(); i++)
		{
			threads.push_back(std::thread(UpdateVlue, i));
		}
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}


}

VOID UpdateWeaponNameThreads()
{
	while (true)
	{
		Sleep(100);

		std::vector<std::thread> threads;
		for (int i = 0; i < EntityList.size(); i++)
		{
			threads.push_back(std::thread(UpdateWeaponName, i));
		}
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

	}
}

VOID KeysCheckThread() {
	while (true)
	{
		Sleep(1);
		MenuKey = ProcessMgr.is_key_down(VK_F8);
		AimKey = ProcessMgr.is_key_down(AimControl::HotKey);
		TriggerKey = ProcessMgr.is_key_down(TriggerBot::HotKey);
	}
}

