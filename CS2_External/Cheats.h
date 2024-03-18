#pragma once

#include "../mapsdata.h"
#include "Game.h"
#include "Entity.h"
#include "AimBot.hpp"
#include "Radar/Radar.h"
#include "TriggerBot.h"
#include "Bunnyhop.hpp"
#include "AntiFlashbang.hpp"

namespace Cheats
{
	void Menu();
	void RadarSetting(Base_Radar& Radar);
	void Run();
	inline CEntity LocalEntityPlayer;

	inline std::vector<CEntity> EntityList;

	inline bool TriggerKey = false;
	inline bool AimKey = false;
	inline bool MenuKey = false;

	inline char mapname[32];
}
