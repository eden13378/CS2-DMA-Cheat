#pragma once
#include "Game.h"
#include "Bone.h"

namespace MenuConfig
{
	inline std::string path = "config";

	inline bool ShowBoneESP = false;
	inline bool ShowBoxESP = true;
	inline bool ShowHealthBar = true;
	inline bool ShowWeaponESP = true;
	inline bool ShowDistance = false;
	inline bool ShowEyeRay = false;
	inline bool ShowPlayerName = true;

	inline bool AimBot = true;
	inline int AimBotHotKey = 4;
	// 0: head 1: neck 3: spine
	inline int  AimPosition = 1;
	inline int  AimPositionPistol = 0;
	inline int  AimPositionSniper = 2;

	inline DWORD  AimPositionIndex = BONEINDEX::neck_0;
	inline DWORD  AimPositionIndexPistol = BONEINDEX::head;
	inline DWORD  AimPositionIndexSniper = BONEINDEX::spine_1;

	inline bool ShowAimFovRange = false;
	inline ImColor AimFovRangeColor = ImColor(230, 230, 230, 255);
	inline bool VisibleCheck = true;
	// 0: normal 1: dynamic
	inline int  BoxType = 0;
	// 0: Vertical 1: Horizontal
	inline int  HealthBarType = 0;

	inline ImColor BoneColor = ImColor(255, 255, 255, 255);
	inline ImColor BoxColor = ImColor(255, 255, 255, 255);
	inline ImColor EyeRayColor = ImColor(255, 0, 0, 255);

	inline bool ShowMenu = true;

	inline bool ShowRadar = false;

	inline int RadarType = 0;

	inline bool TriggerBot = true;
	inline int TriggerHotKey = 4;
	inline int TriggerMode = 0;
	inline bool Pressed = false;// for toggle mode
	inline bool Shoot = false;// so that it doesn’t aim when the trigger hits

	inline bool TeamCheck = true;

	inline bool ShowLineToEnemy = false;
	inline ImColor LineToEnemyColor = ImColor(255, 255, 255, 220);

}

namespace GunList {
	inline std::vector <std::string> pistolsList = { "hkp2000", "glock", "cz75a", "deagle", "elite", "fiveseven", "p250", "revolver", "tec9", "usp_silencer" };

	inline std::vector <std::string> snipersList = { "awp", "g3sg1", "scar20", "ssg08" };
}

namespace KmBox {
	inline std::string type;
}