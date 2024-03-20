#pragma once
#include "KMbox/KmBoxNetManager.h"
#include "KMbox/KmboxB.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include "TriggerBot.h"
#include <iostream>

extern "C" {
#include "Entity.h"
}


namespace AimControl
{
    inline int HotKey = VK_XBUTTON2;
    inline float AimFov = 6;
    inline float FakeSmooth = 0.8;
    inline float Smooth = 0.3;

    inline std::vector<int> HotKeyList{VK_LBUTTON, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL};// added new button VK_LBUTTON

    
    inline bool AutoShot = false;
    inline bool ignoreOnShot = true;

    inline void SetHotKey(int Index)
    {
        HotKey = HotKeyList.at(Index);
    }

    inline void AimBot(const CEntity& Local, Vec3 LocalPos, Vec3 AimPos)
    {
        if (Local.Pawn.WeaponName == "knife") return;
        if (ignoreOnShot && ProcessMgr.is_key_down(VK_LBUTTON)) return; // need to make var
        float Yaw, Pitch;
        float Distance, Norm, Length;
        Vec3 OppPos;
        Vec2 Angles{ 0,0 };
        int ScreenCenterX = Gui.Window.Size.x / 2;
        int ScreenCenterY = Gui.Window.Size.y / 2;
        float TargetX = 0.f;
        float TargetY = 0.f;
        OppPos = AimPos - LocalPos;

        Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

        Length = sqrt(Distance * Distance + OppPos.z * OppPos.z);

        Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.Pawn.ViewAngle.y;
        Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.Pawn.ViewAngle.x;
        Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));

        Vec2 ScreenPos;
        gGame.View.WorldToScreen(Vec3(AimPos), ScreenPos);
        if (Norm < AimFov)
        {
            // Shake Fixed by @Sweely

            if (ScreenPos.x != ScreenCenterX)
            {
                TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x) : ScreenPos.x - ScreenCenterX;
                TargetX /= FakeSmooth != 0.0f ? FakeSmooth : 1.5f;
                TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 || TargetX + ScreenCenterX < 0) ? 0 : TargetX;
            }

            if (ScreenPos.y != 0)
            {
                if (ScreenPos.y != ScreenCenterY)
                {
                    TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y) : ScreenPos.y - ScreenCenterY;
                    TargetY /= FakeSmooth != 0.0f ? FakeSmooth : 1.5f;
                    TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 || TargetY + ScreenCenterY < 0) ? 0 : TargetY;
                }
            }

            float DistanceRatio = Norm / AimFov; // Calculate the distance ratio
            float SpeedFactor = 1.0f + (1.0f - DistanceRatio); // Determine the speed factor based on the distance ratio
            TargetX /= (FakeSmooth * SpeedFactor);
            TargetY /= (FakeSmooth * SpeedFactor);

            if (ScreenPos.x != ScreenCenterX)
            {
                TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x) : ScreenPos.x - ScreenCenterX;
                TargetX /= FakeSmooth != 0.0f ? FakeSmooth : 1.5f;
                TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 || TargetX + ScreenCenterX < 0) ? 0 : TargetX;
            }

            if (ScreenPos.y != 0)
            {
                if (ScreenPos.y != ScreenCenterY)
                {
                    TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y) : ScreenPos.y - ScreenCenterY;
                    TargetY /= FakeSmooth != 0.0f ? FakeSmooth : 1.5f;
                    TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 || TargetY + ScreenCenterY < 0) ? 0 : TargetY;
                }
            }

            if (Smooth) {
                if (KmBox::type == "net") KmBoxMgr.Mouse.Move_Auto(TargetX, TargetY, 60 * Smooth);
                if (KmBox::type == "b") kmBoxBMgr.km_move_auto(TargetX, TargetY, 60 * Smooth);
            }
            else {
                if (KmBox::type == "net") KmBoxMgr.Mouse.Move(TargetX, TargetY);
                if (KmBox::type == "b") kmBoxBMgr.km_move(TargetX, TargetY);
            }

            if (AutoShot) {
                TriggerBot::Run(Local);
            }
        }
    }
}