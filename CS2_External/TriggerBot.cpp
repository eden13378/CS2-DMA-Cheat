#include "TriggerBot.h"

void TriggerBot::Run(const CEntity& LocalEntity)
{
	DWORD uHandle = 0;
	if (!ProcessMgr.ReadMemory<DWORD>(LocalEntity.Pawn.Address + Offset::iIDEntIndex, uHandle))
		return;
	if (uHandle == -1)
		return;

	DWORD64 ListEntry = 0;
	ListEntry = ProcessMgr.TraceAddress(gGame.GetEntityListAddress(), { 0x8 * (uHandle >> 9) + 0x10,0x0 });
	if (ListEntry == 0)
		return;

	DWORD64 PawnAddress = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(ListEntry + 0x78 * (uHandle & 0x1FF), PawnAddress))
		return;

	CEntity Entity;
	if (!Entity.UpdatePawn(PawnAddress))
		return;

	bool AllowShoot = false;

	if (MenuConfig::TeamCheck)
		AllowShoot = LocalEntity.Pawn.TeamID != Entity.Pawn.TeamID && Entity.Pawn.Health > 0;
	else
		AllowShoot = Entity.Pawn.Health > 0;

	if (!AllowShoot)
		return;

	static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
	auto CurTimePoint = std::chrono::steady_clock::now();
	if (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(TriggerDelay))
	{
		if (!ProcessMgr.is_key_down(VK_LBUTTON))
		{
			if (KmBox::type == "net") {
				KmBoxMgr.Mouse.Left(true);
				KmBoxMgr.Mouse.Left(false);
			} else if (KmBox::type == "net") {
				kmBoxBMgr.km_click();
			}
		}
		LastTimePoint = CurTimePoint;
	}
}