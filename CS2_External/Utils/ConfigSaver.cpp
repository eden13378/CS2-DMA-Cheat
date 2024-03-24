#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ConfigSaver.hpp"
#include "../MenuConfig.hpp" // Include your global settings header
#include "../mapsdata.h"
#include "../TriggerBot.h"
#include "../AimBot.hpp"

namespace MyConfigSaver {

    // Function to save the configuration to a file
    void SaveConfig(const std::string& filename) {
        std::ofstream configFile(MenuConfig::path+'/'+filename);
        if (!configFile.is_open()) {
            std::cerr << "Error: Could not open the configuration file." << std::endl;
            return;
        }
       
        // Example: Save global settings to the file
        configFile << "ShowBoneESP " << MenuConfig::ShowBoneESP << std::endl;
        configFile << "TriggerDelay " << TriggerBot::TriggerDelay << std::endl;
        configFile << "ShowBoxESP " << MenuConfig::ShowBoxESP << std::endl;
        configFile << "TriggerHotKey " << MenuConfig::TriggerHotKey << std::endl;
        configFile << "TriggerMode " << MenuConfig::TriggerMode << std::endl;
        configFile << "ShowHealthBar " << MenuConfig::ShowHealthBar << std::endl;
        configFile << "AimFov " << AimControl::AimFov << std::endl;
        configFile << "AimBotHotKey " << MenuConfig::AimBotHotKey << std::endl;
        configFile << "ShowLineToEnemy " << MenuConfig::ShowLineToEnemy << std::endl;
        configFile << "ShowWeaponESP " << MenuConfig::ShowWeaponESP << std::endl;
        configFile << "ShowDistance " << MenuConfig::ShowDistance << std::endl;
        configFile << "Smooth " << AimControl::Smooth << std::endl;
        configFile << "ShowEyeRay " << MenuConfig::ShowEyeRay << std::endl;
        configFile << "ShowPlayerName " << MenuConfig::ShowPlayerName << std::endl;
        configFile << "AimBot " << MenuConfig::AimBot << std::endl;
        configFile << "AimPosition " << MenuConfig::AimPosition << std::endl;
        configFile << "AimPositionIndex " << MenuConfig::AimPositionIndex << std::endl;
        configFile << "AimPositionPistol " << MenuConfig::AimPositionPistol << std::endl;
        configFile << "AimPositionIndexPistol " << MenuConfig::AimPositionIndexPistol << std::endl;
        configFile << "AimPositionSniper " << MenuConfig::AimPositionSniper << std::endl;
        configFile << "AimPositionIndexSniper " << MenuConfig::AimPositionIndexSniper << std::endl;
        configFile << "AimPosition " << MenuConfig::AimPosition << std::endl;
        configFile << "AimIgnoreOnShot " << AimControl::ignoreOnShot << std::endl;
        configFile << "AimPositionIndex " << MenuConfig::AimPositionIndex << std::endl;
        configFile << "HealthBarType " << MenuConfig::HealthBarType << std::endl;
        configFile << "BoneColor " << MenuConfig::BoneColor.Value.x << " " << MenuConfig::BoneColor.Value.y << " " << MenuConfig::BoneColor.Value.z << " " << MenuConfig::BoneColor.Value.w << std::endl;
        configFile << "LineToEnemyColor " << MenuConfig::LineToEnemyColor.Value.x << " " << MenuConfig::LineToEnemyColor.Value.y << " " << MenuConfig::LineToEnemyColor.Value.z << " " << MenuConfig::LineToEnemyColor.Value.w << std::endl;
        configFile << "BoxColor " << MenuConfig::BoxColor.Value.x << " " << MenuConfig::BoxColor.Value.y << " " << MenuConfig::BoxColor.Value.z << " " << MenuConfig::BoxColor.Value.w << std::endl;
        configFile << "EyeRayColor " << MenuConfig::EyeRayColor.Value.x << " " << MenuConfig::EyeRayColor.Value.y << " " << MenuConfig::EyeRayColor.Value.z << " " << MenuConfig::EyeRayColor.Value.w << std::endl;
        configFile << "ShowMenu " << MenuConfig::ShowMenu << std::endl;
        configFile << "ShowRadar " << MenuConfig::ShowRadar << std::endl;
        configFile << "RadarType " << MenuConfig::RadarType << std::endl;
        configFile << "RadarSize " << mp::RadarSize << std::endl;
        configFile << "RadarLineLenght " << mp::LineLenght << std::endl;
        configFile << "RadarCircleSize " << mp::CircleSize << std::endl;
        configFile << "BoxType " << MenuConfig::BoxType << std::endl;
        configFile << "TriggerBot " << MenuConfig::TriggerBot << std::endl;
        configFile << "TeamCheck " << MenuConfig::TeamCheck << std::endl;
		configFile << "VisibleCheck " << MenuConfig::VisibleCheck << std::endl;
        configFile << "ShowAimFovRange " << MenuConfig::ShowAimFovRange << std::endl;
        configFile << "AimFovRangeColor " << MenuConfig::AimFovRangeColor.Value.x << " " << MenuConfig::AimFovRangeColor.Value.y << " " << MenuConfig::AimFovRangeColor.Value.z << " " << MenuConfig::AimFovRangeColor.Value.w << std::endl;
        configFile.close();
    }

    // Function to load the configuration from a file
    void LoadConfig(const std::string& filename) {
        std::ifstream configFile(MenuConfig::path + '/' + filename);
        if (!configFile.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (iss >> key) {
                if (key == "ShowBoneESP") iss >> MenuConfig::ShowBoneESP;
                else if (key == "TriggerDelay") iss >> TriggerBot::TriggerDelay;
                else if (key == "ShowBoxESP") iss >> MenuConfig::ShowBoxESP;
                else if (key == "TriggerHotKey") { iss >> MenuConfig::TriggerHotKey; TriggerBot::SetHotKey(MenuConfig::TriggerHotKey); }
                else if (key == "TriggerMode") { iss >> MenuConfig::TriggerMode; TriggerBot::SetMode(MenuConfig::TriggerMode); }
                else if (key == "ShowHealthBar") iss >> MenuConfig::ShowHealthBar;
                else if (key == "AimFov") iss >> AimControl::AimFov;
                else if (key == "AimBotHotKey") { iss >> MenuConfig::AimBotHotKey; AimControl::SetHotKey(MenuConfig::AimBotHotKey); }
                else if (key == "ShowLineToEnemy") iss >> MenuConfig::ShowLineToEnemy;
                else if (key == "ShowWeaponESP") iss >> MenuConfig::ShowWeaponESP;
                else if (key == "ShowDistance") iss >> MenuConfig::ShowDistance;
                else if (key == "Smooth") iss >> AimControl::Smooth;
                else if (key == "ShowEyeRay") iss >> MenuConfig::ShowEyeRay;
                else if (key == "ShowPlayerName") iss >> MenuConfig::ShowPlayerName;
                else if (key == "AimBot") iss >> MenuConfig::AimBot;
                else if (key == "AimPosition") iss >> MenuConfig::AimPosition;
                else if (key == "AimPositionIndex") iss >> MenuConfig::AimPositionIndex;
                else if (key == "AimPositionPistol") iss >> MenuConfig::AimPositionPistol;
                else if (key == "AimPositionIndexPistol") iss >> MenuConfig::AimPositionIndexPistol;
                else if (key == "AimPositionSniper") iss >> MenuConfig::AimPosition;
                else if (key == "AimPositionIndexSniper") iss >> MenuConfig::AimPositionIndex;
                else if (key == "AimIgnoreOnShot") iss >> AimControl::ignoreOnShot;
                else if (key == "HealthBarType") iss >> MenuConfig::HealthBarType;
                else if (key == "BoneColor") iss >> MenuConfig::BoneColor.Value.x >> MenuConfig::BoneColor.Value.y >> MenuConfig::BoneColor.Value.z >> MenuConfig::BoneColor.Value.w;
                else if (key == "LineToEnemyColor") iss >> MenuConfig::LineToEnemyColor.Value.x >> MenuConfig::LineToEnemyColor.Value.y >> MenuConfig::LineToEnemyColor.Value.z >> MenuConfig::LineToEnemyColor.Value.w;
                else if (key == "BoxColor") iss >> MenuConfig::BoxColor.Value.x >> MenuConfig::BoxColor.Value.y >> MenuConfig::BoxColor.Value.z >> MenuConfig::BoxColor.Value.w;
                else if (key == "EyeRayColor") iss >> MenuConfig::EyeRayColor.Value.x >> MenuConfig::EyeRayColor.Value.y >> MenuConfig::EyeRayColor.Value.z >> MenuConfig::EyeRayColor.Value.w;
                else if (key == "ShowMenu") iss >> MenuConfig::ShowMenu;
                else if (key == "ShowRadar") iss >> MenuConfig::ShowRadar;
                else if (key == "RadarType") iss >> MenuConfig::RadarType;
                else if (key == "RadarSize") iss >> mp::RadarSize;
                else if (key == "RadarLineLenght") iss >> mp::LineLenght;
                else if (key == "RadarCircleSize") iss >> mp::CircleSize;
                else if (key == "BoxType") iss >> MenuConfig::BoxType;
                else if (key == "TriggerBot") iss >> MenuConfig::TriggerBot;
                else if (key == "TeamCheck") iss >> MenuConfig::TeamCheck;
                else if (key == "VisibleCheck") iss >> MenuConfig::VisibleCheck;
                else if (key == "ShowAimFovRange") iss >> MenuConfig::ShowAimFovRange;
                else if (key == "AimFovRangeColor") iss >> MenuConfig::AimFovRangeColor.Value.x >> MenuConfig::AimFovRangeColor.Value.y >> MenuConfig::AimFovRangeColor.Value.z >> MenuConfig::AimFovRangeColor.Value.w;
            }
        }

        configFile.close();
    }
} // namespace ConfigSaver
