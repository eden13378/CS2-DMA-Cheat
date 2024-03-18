#include "ConfigMenu.hpp"
#include "../MenuConfig.hpp"
#include "ConfigSaver.hpp"
#include "../TriggerBot.h"
#include "../AimBot.hpp"
#include <filesystem>

namespace ConfigMenu {

    void RenderConfigMenu() {

		if (ImGui::BeginTabItem("Configs"))
		{
			static char configNameBuffer[128] = "";

			ImGui::InputText("New config", configNameBuffer, sizeof(configNameBuffer));

			if (ImGui::Button("Create"))
			{
				std::string configFileName = std::string(configNameBuffer) + ".config";
				MyConfigSaver::SaveConfig(configFileName);
			}

			ImGui::Separator();

			static int selectedConfig = -1;

			const std::string configDir = MenuConfig::path;
			static std::vector<std::string> configFiles;

			configFiles.clear();
			for (const auto& entry : std::filesystem::directory_iterator(configDir))
			{
				if (entry.is_regular_file() && entry.path().extension() == ".config")
				{
					configFiles.push_back(entry.path().filename().string());
				}
			}

			for (int i = 0; i < configFiles.size(); ++i)
			{
				if (ImGui::Selectable(configFiles[i].c_str(), selectedConfig == i))
				{
					selectedConfig = i;
				}
			}

			if (ImGui::Button("Load config") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				MyConfigSaver::LoadConfig(selectedConfigFile);
			}

			ImGui::SameLine();

			if (ImGui::Button("Save config") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				MyConfigSaver::SaveConfig(selectedConfigFile);
			}

			ImGui::SameLine();

			if (ImGui::Button("Delete config") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				std::string fullPath = configDir + "/" + selectedConfigFile;
				if (std::remove(fullPath.c_str()) == 0)
				{
					configFiles.erase(configFiles.begin() + selectedConfig);
					selectedConfig = -1;
				}
			}

			ImGui::EndTabItem();
		}
    }

}
