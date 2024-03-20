#include "Cheats.h"
#include "Render.hpp"
#include "MenuConfig.hpp"
#include "Utils/ConfigMenu.hpp"
#include "Utils/ConfigSaver.hpp"

inline ImFont* myFont = nullptr;


void setStyles() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.06f, 0.08f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.10f, 0.11f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

void Cheats::Menu()
{
	static bool IsMenuInit = false;
	if (!IsMenuInit)
	{
		setStyles();
		IsMenuInit = true;
	}

	ImGui::PushFont(myFont);
	ImGui::Begin("CS 2 / DMA", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::BeginTabBar("settings");
		// esp menu
		if (ImGui::BeginTabItem("Visuals"))
		{
			Gui.MyCheckBox("Show box", &MenuConfig::ShowBoxESP);
			ImGui::SameLine();
			ImGui::ColorEdit4("Box color", reinterpret_cast<float*>(&MenuConfig::BoxColor), ImGuiColorEditFlags_NoInputs);

			ImGui::Combo("Box type", &MenuConfig::BoxType, "Normal\0Slim");
			
			Gui.MyCheckBox("Show bone", &MenuConfig::ShowBoneESP);
			ImGui::SameLine();
			ImGui::ColorEdit4("Bone color", reinterpret_cast<float*>(&MenuConfig::BoneColor), ImGuiColorEditFlags_NoInputs);

			Gui.MyCheckBox("Show eye ray", &MenuConfig::ShowEyeRay);
			ImGui::SameLine();
			ImGui::ColorEdit4("Eye ray color", reinterpret_cast<float*>(&MenuConfig::EyeRayColor), ImGuiColorEditFlags_NoInputs);

			Gui.MyCheckBox("Show bar", &MenuConfig::ShowHealthBar);
			ImGui::Combo("Health bar position", &MenuConfig::HealthBarType, "Left\0Up");

			Gui.MyCheckBox("Weapon esp", &MenuConfig::ShowWeaponESP);
			Gui.MyCheckBox("Show distance", &MenuConfig::ShowDistance);
			Gui.MyCheckBox("Show name", &MenuConfig::ShowPlayerName);
		
			Gui.MyCheckBox("Show line to enemy", &MenuConfig::ShowLineToEnemy);
			ImGui::SameLine();
			ImGui::ColorEdit4("Line color", reinterpret_cast<float*>(&MenuConfig::LineToEnemyColor), ImGuiColorEditFlags_NoInputs);

			ImGui::EndTabItem();
		}

		// aimbot menu
		if (ImGui::BeginTabItem("Aimbot"))
		{
			Gui.MyCheckBox("Enable", &MenuConfig::AimBot);

			if (ImGui::Combo("Aimbot hotkey", &MenuConfig::AimBotHotKey, "LBUTTON\0MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL"))// added LBUTTON
			{
				AimControl::SetHotKey(MenuConfig::AimBotHotKey);
			}

			float FovMin = 0.1f, FovMax = 89.f;
			float SmoothMin = 0.f, SmoothMax = 0.9f;
			Gui.SliderScalarEx1("fov", ImGuiDataType_Float, &AimControl::AimFov, &FovMin, &FovMax, "%.1f", ImGuiSliderFlags_None);
			Gui.MyCheckBox("Show aimfov", &MenuConfig::ShowAimFovRange);
			ImGui::SameLine();
			ImGui::ColorEdit4("Aimfov color", reinterpret_cast<float*>(&MenuConfig::AimFovRangeColor), ImGuiColorEditFlags_NoInputs);

			Gui.SliderScalarEx1("Smooth", ImGuiDataType_Float, &AimControl::Smooth, &SmoothMin, &SmoothMax, "%.1f", ImGuiSliderFlags_None);
			if (ImGui::Combo("Aim position (default)", &MenuConfig::AimPosition, "Head\0Neck\0Spine\0"))
			{
				switch (MenuConfig::AimPosition)
				{
				case 0:
					MenuConfig::AimPositionIndex = BONEINDEX::head;
					break;
				case 1:
					MenuConfig::AimPositionIndex = BONEINDEX::neck_0;
					break;
				case 2:
					MenuConfig::AimPositionIndex = BONEINDEX::spine_1;
					break;
				default:
					break;
				}
			}
			if (ImGui::Combo("Aim position (pistol)", &MenuConfig::AimPositionPistol, "Head\0Neck\0Spine\0"))
			{
				switch (MenuConfig::AimPositionPistol)
				{
				case 0:
					MenuConfig::AimPositionIndexPistol = BONEINDEX::head;
					break;
				case 1:
					MenuConfig::AimPositionIndexPistol = BONEINDEX::neck_0;
					break;
				case 2:
					MenuConfig::AimPositionIndexPistol = BONEINDEX::spine_1;
					break;
				default:
					break;
				}
			}

			if (ImGui::Combo("Aim position (sniper)", &MenuConfig::AimPositionSniper, "Head\0Neck\0Spine\0"))
			{
				switch (MenuConfig::AimPositionSniper)
				{
				case 0:
					MenuConfig::AimPositionIndexSniper = BONEINDEX::head;
					break;
				case 1:
					MenuConfig::AimPositionIndexSniper = BONEINDEX::neck_0;
					break;
				case 2:
					MenuConfig::AimPositionIndexSniper = BONEINDEX::spine_1;
					break;
				default:
					break;
				}
			}
			Gui.MyCheckBox("AutoShot", &AimControl::AutoShot);

			ImGui::SameLine();

			Gui.MyCheckBox("Visible check", &MenuConfig::VisibleCheck);
			Gui.MyCheckBox("Ignore on shot", &AimControl::ignoreOnShot);

			ImGui::EndTabItem();
		}

		// Radar menu
		if (ImGui::BeginTabItem("Radar"))
		{
			Gui.MyCheckBox("Show radar", &MenuConfig::ShowRadar);
			if (ImGui::Combo("Radar size", &MenuConfig::RadarType, "Small\0Big")) {
				switch (MenuConfig::RadarType)
				{
				case 0:
					mp::CircleSize = 2; mp::LineLenght = 6; mp::RadarSize = 300;
					break;
				case 1:
					mp::CircleSize = 5; mp::LineLenght = 10; mp::RadarSize = 600;
					break;
				default:
					break;
				}
			}
			ImGui::EndTabItem();
		}

		// TriggerBot
		if (ImGui::BeginTabItem("Triggerbot"))
		{
			Gui.MyCheckBox("Enable", &MenuConfig::TriggerBot);

			if (ImGui::Combo("Trigger Hotkey", &MenuConfig::TriggerHotKey, "MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL"))
			{
				TriggerBot::SetHotKey(MenuConfig::TriggerHotKey);
			}
			if (ImGui::Combo("TriggerBot mode", &MenuConfig::TriggerMode, "OnKey\0Always"))
			{
				TriggerBot::SetMode(MenuConfig::TriggerMode);
			}

			DWORD TriggerDelayMin = 0, TriggerDelayMax = 250;
			Gui.SliderScalarEx1("Delay", ImGuiDataType_U32, &TriggerBot::TriggerDelay, &TriggerDelayMin, &TriggerDelayMax, "%d", ImGuiSliderFlags_None);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Utilities"))
		{
			// TeamCheck
			Gui.MyCheckBox("Team check", &MenuConfig::TeamCheck);

			static bool buffer = false;

			if (ImGui::Checkbox("Close hack", &buffer)) {
				Gui.EndFlag = true;
				//exit(-1);
			}
			ImGui::EndTabItem();

		}

		// Render config saver
		ConfigMenu::RenderConfigMenu();
		
		ImGui::Separator();

		ImGui::EndTabBar();
	}ImGui::End();
	ImGui::PopFont();
}

void Cheats::RadarSetting(Base_Radar& Radar)
{
	Radar.UpdateMap(mapname);
	// Radar window
	ImGui::SetNextWindowBgAlpha(0.1f);
	ImGui::Begin("Radar", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowSize({ mp::RadarSize, mp::RadarSize });

	Radar.SetDrawList(ImGui::GetWindowDrawList());

	Radar.SetPos({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y });
	Radar.Opened = true;
}

void Cheats::Run()
{
	// Show menu
	static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
	auto CurTimePoint = std::chrono::steady_clock::now();

	if (MenuKey
		&& CurTimePoint - LastTimePoint >= std::chrono::milliseconds(150))
	{
		// Check key state per 150ms once to avoid loop.
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
		LastTimePoint = CurTimePoint;
	}

	if (MenuConfig::ShowMenu)
		Menu();

	

	// HealthBar Map
	static std::map<DWORD64, Render::HealthBar> HealthBarMap;

	// AimBot data
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  HeadPos{ 0,0,0 };
	Vec3  AimPos{ 0,0,0 };

	// Radar Data
	Base_Radar Radar;
	
	if (MenuConfig::ShowRadar)
		RadarSetting(Radar);


	for (int i = 0; i < EntityList.size(); i++)
	{
		CEntity Entity = EntityList[i];
		if (Entity.Pawn.Health <= 0) {
			continue;
		}
		
		// Add entity to radar
		if (MenuConfig::ShowRadar)
			Radar.AddPoint(LocalEntityPlayer.Pawn.Pos, Entity.Pawn.ViewAngle.y, Entity.Pawn.Pos, ImColor(237, 85, 106, 200), Entity.Pawn.ViewAngle.y, (int)(Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID), Entity.Pawn.Health);


		if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID)
			continue;

		if (!Entity.IsInScreen())
			continue;

		DistanceToSight = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos.DistanceTo({ Gui.Window.Size.x / 2,Gui.Window.Size.y / 2 });


		if (DistanceToSight < MaxAimDistance)
		{
			MaxAimDistance = DistanceToSight;
			if (!MenuConfig::VisibleCheck ||
				Entity.Pawn.bSpottedByMask & (DWORD64(1) << (LocalEntityPlayer.LocalPlayerControllerIndex)) ||
				LocalEntityPlayer.Pawn.bSpottedByMask & (DWORD64(1) << (i)))
			{
				if (std::find(GunList::pistolsList.begin(), GunList::pistolsList.end(), LocalEntityPlayer.Pawn.WeaponName) != GunList::pistolsList.end()) {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndexPistol].Pos;
					if (MenuConfig::AimPositionIndexPistol == BONEINDEX::head)
						AimPos.z -= 1.f;
				} else if (std::find(GunList::snipersList.begin(), GunList::snipersList.end(), LocalEntityPlayer.Pawn.WeaponName) != GunList::snipersList.end()) {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndexSniper].Pos;
					if (MenuConfig::AimPositionIndexSniper == BONEINDEX::head)
						AimPos.z -= 1.f;
				}
				else {
					AimPos = Entity.GetBone().BonePosList[MenuConfig::AimPositionIndex].Pos;
					if (MenuConfig::AimPositionIndex == BONEINDEX::head)
						AimPos.z -= 1.f;
				}
			}
		}

		// Draw Bone
		if (MenuConfig::ShowBoneESP)
			Render::DrawBone(Entity, MenuConfig::BoneColor, 1.3);

		// Draw eyeRay
		if (MenuConfig::ShowEyeRay)
			Render::ShowLosLine(Entity, 50, MenuConfig::EyeRayColor, 1.3);

		// Box
		ImVec4 Rect;
		switch (MenuConfig::BoxType)
		{
		case 0:
			Rect = Render::Get2DBox(Entity);
			break;
		case 1:
			Rect = Render::Get2DBoneRect(Entity);
			break;
		default:
			break;
		}

		// Line to enemy
		if (MenuConfig::ShowLineToEnemy)
			Render::LineToEnemy(Rect, MenuConfig::LineToEnemyColor, 1.2);

		// Draw Box
		if (MenuConfig::ShowBoxESP)
			Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, MenuConfig::BoxColor, 1.3);

		// Draw HealthBar
		if (MenuConfig::ShowHealthBar)
		{
			ImVec2 HealthBarPos, HealthBarSize;
			if (MenuConfig::HealthBarType == 0)
			{
				// Vertical
				HealthBarPos = { Rect.x - 7.f,Rect.y };
				HealthBarSize = { 7 ,Rect.w };
			}
			else
			{
				// Horizontal
				HealthBarPos = { Rect.x + Rect.z / 2 - 70 / 2,Rect.y - 13 };
				HealthBarSize = { 70,8 };
			}
			Render::DrawHealthBar(Entity.Controller.Address, 100, Entity.Pawn.Health, HealthBarPos, HealthBarSize, MenuConfig::HealthBarType);
		}

		// Draw weaponName
		if (MenuConfig::ShowWeaponESP)
			Gui.StrokeText(Entity.Pawn.WeaponName, { Rect.x,Rect.y + Rect.w }, ImColor(255, 255, 255, 255), 14);

		if (MenuConfig::ShowDistance)
			Render::DrawDistance(LocalEntityPlayer, Entity, Rect);

		if (MenuConfig::ShowPlayerName)
		{
			if (MenuConfig::HealthBarType == 0)
				Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 14 }, ImColor(255, 255, 255, 255), 14, true);
			else
				Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 13 - 14 }, ImColor(255, 255, 255, 255), 14, true);
		}

	}

	if (MenuConfig::ShowRadar && LocalEntityPlayer.IsAlive())
		Radar.AddPoint(LocalEntityPlayer.Pawn.Pos, LocalEntityPlayer.Pawn.ViewAngle.y, LocalEntityPlayer.Pawn.Pos, ImColor(237, 85, 106, 200), LocalEntityPlayer.Pawn.ViewAngle.y, 2, LocalEntityPlayer.Pawn.Health);


	// Radar render
	if (MenuConfig::ShowRadar)
	{
		Radar.Render();
		ImGui::End();
	}

	if (MenuConfig::TriggerMode == 0 && MenuConfig::TriggerBot && TriggerKey)
	{
		MenuConfig::Shoot = true;
		TriggerBot::Run(LocalEntityPlayer);
		MenuConfig::Shoot = false;
	}

	else if (MenuConfig::TriggerMode == 1 && MenuConfig::TriggerBot) 
	{
		MenuConfig::Shoot = true;
		TriggerBot::Run(LocalEntityPlayer);
		MenuConfig::Shoot = false;
	}

	// Fov circle
	if(MenuConfig::ShowAimFovRange)
		Render::DrawFovCircle(LocalEntityPlayer);
	

	if (MenuConfig::AimBot && AimKey)
	{
		if (AimPos != Vec3(0, 0, 0))
		{

			AimControl::AimBot(LocalEntityPlayer, LocalEntityPlayer.Pawn.CameraPos, AimPos);
		}
	}
}
