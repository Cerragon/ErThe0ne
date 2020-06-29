#include "ErectusInclude.h"
#include "fmt/format.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

void ErectusImGui::ProcessMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ErectusMain::windowSize[0]), static_cast<float>(ErectusMain::windowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	auto allowDrag = true;
	if (ImGui::Begin("Erectus - Process Menu", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
				ErectusMain::CloseWnd();
			if (ImGui::MenuItem("Overlay Menu"))
				ErectusMain::SetOverlayMenu();
			if (!ErectusProcess::pid)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

				ImGui::MenuItem("Overlay");

				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
			}
			else
			{
				if (ImGui::MenuItem("Overlay Menu"))
					ErectusMain::SetOverlayMenu();
				if (ImGui::MenuItem("Overlay"))
					ErectusMain::SetOverlayPosition(false, true);
			}
			ImGui::EndMenuBar();
		}

		ImGui::SetNextItemWidth(-16.f);

		auto processText = ErectusProcess::pid ? "Fallout76.exe - " + std::to_string(ErectusProcess::pid) : "No  process selected.";
		if (ImGui::BeginCombo("###ProcessList", processText.c_str()))
		{
			for (auto item : ErectusProcess::GetProcesses())
			{
				processText = item ? "Fallout76.exe - " + std::to_string(item) : "NONE";
				if (ImGui::Selectable(processText.c_str()))
					ErectusProcess::AttachToProcess(item);
			}

			ImGui::EndCombo();
			allowDrag = false;
		}

		ImGui::Separator();
		switch (ErectusProcess::processErrorId)
		{
		case 0:
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ErectusProcess::processError.c_str());
			break;
		case 1:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ErectusProcess::processError.c_str());
			break;
		case 2:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ErectusProcess::processError.c_str());
			break;
		default:
			ImGui::Text(ErectusProcess::processError.c_str());
			break;
		}

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::Text("Overlay Menu Keybind");
		ImGui::NextColumn();
		ImGui::Text("CTRL+ENTER");
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("ImGui (D3D9) FPS");
		ImGui::NextColumn();
		ImGui::Text("%.1f", ImGui::GetIO().Framerate);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("PID (Process Id)");
		ImGui::NextColumn();
		ImGui::Text("%lu", ErectusProcess::pid);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("HWND (Window)");
		ImGui::NextColumn();
		ImGui::Text("%p", ErectusProcess::hWnd);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("Base Address");
		ImGui::NextColumn();
		ImGui::Text("%016llX", ErectusProcess::exe);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("HANDLE");
		ImGui::NextColumn();
		ImGui::Text("%p", ErectusProcess::handle);
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopItemFlag();
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ErectusImGui::ButtonToggle(const char* label, bool* state)
{
	if (*state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(224.0f, 0.0f)))
			*state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(224.0f, 0.0f)))
			*state = true;
		ImGui::PopStyleColor(3);
	}
}

void ErectusImGui::LargeButtonToggle(const char* label, bool* state)
{
	if (*state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(451.0f, 0.0f)))
			*state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(451.0f, 0.0f)))
			*state = true;
		ImGui::PopStyleColor(3);
	}
}

void ErectusImGui::SmallButtonToggle(const char* label, bool* state)
{
	if (*state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f)))
			*state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f)))
			*state = true;
		ImGui::PopStyleColor(3);
	}
}

void ErectusImGui::OverlayMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ErectusMain::windowSize[0]), static_cast<float>(ErectusMain::windowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	if (ImGui::Begin("Erectus - Overlay Menu", nullptr,
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
				ErectusMain::CloseWnd();

			if (ImGui::MenuItem("Process Menu"))
				ErectusProcess::SetProcessMenu();

			if (ImGui::MenuItem("Overlay"))
			{
				if (!ErectusMain::SetOverlayPosition(false, true))
					ErectusProcess::SetProcessMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("###OverlayMenuTabBar", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("ESP###ESPTab"))
			{
				if (ImGui::CollapsingHeader("Player ESP Settings"))
				{
					ButtonToggle("Player ESP Enabled", &ErectusIni::playerSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###PlayerSettingsEnabledDistance", &ErectusIni::playerSettings.enabledDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Draw Living Players", &ErectusIni::playerSettings.drawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsAliveColor", ErectusIni::playerSettings.aliveColor);
					Utils::ValidateRgb(ErectusIni::playerSettings.aliveColor);

					ButtonToggle("Draw Downed Players", &ErectusIni::playerSettings.drawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDownedColor", ErectusIni::playerSettings.downedColor);
					Utils::ValidateRgb(ErectusIni::playerSettings.downedColor);

					ButtonToggle("Draw Dead Players", &ErectusIni::playerSettings.drawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDeadColor", ErectusIni::playerSettings.deadColor);
					Utils::ValidateRgb(ErectusIni::playerSettings.deadColor);

					ButtonToggle("Draw Unknown Players", &ErectusIni::playerSettings.drawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsUnknownColor", ErectusIni::playerSettings.unknownColor);
					Utils::ValidateRgb(ErectusIni::playerSettings.unknownColor);

					ButtonToggle("Draw Enabled Players", &ErectusIni::playerSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsEnabledAlpha", &ErectusIni::playerSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Players", &ErectusIni::playerSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsDisabledAlpha", &ErectusIni::playerSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Players", &ErectusIni::playerSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Players", &ErectusIni::playerSettings.drawUnnamed);

					ButtonToggle("Show Player Name", &ErectusIni::playerSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Player Distance", &ErectusIni::playerSettings.showDistance);

					ButtonToggle("Show Player Health", &ErectusIni::playerSettings.showHealth);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Dead Player Health", &ErectusIni::playerSettings.showDeadHealth);

					ButtonToggle("Player Text Shadowed", &ErectusIni::playerSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Player Text Centered", &ErectusIni::playerSettings.textCentered);
				}

				if (ImGui::CollapsingHeader("NPC ESP Settings"))
				{
					ButtonToggle("NPC ESP Enabled", &ErectusIni::npcSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###NpcSettingsEnabledDistance", &ErectusIni::npcSettings.enabledDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Draw Living NPCs", &ErectusIni::npcSettings.drawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsAliveColor", ErectusIni::npcSettings.aliveColor);
					Utils::ValidateRgb(ErectusIni::npcSettings.aliveColor);

					ButtonToggle("Draw Downed NPCs", &ErectusIni::npcSettings.drawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDownedColor", ErectusIni::npcSettings.downedColor);
					Utils::ValidateRgb(ErectusIni::npcSettings.downedColor);

					ButtonToggle("Draw Dead NPCs", &ErectusIni::npcSettings.drawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDeadColor", ErectusIni::npcSettings.deadColor);
					Utils::ValidateRgb(ErectusIni::npcSettings.deadColor);

					ButtonToggle("Draw Unknown NPCs", &ErectusIni::npcSettings.drawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsUnknownColor", ErectusIni::npcSettings.unknownColor);
					Utils::ValidateRgb(ErectusIni::npcSettings.unknownColor);

					ButtonToggle("Draw Enabled NPCs", &ErectusIni::npcSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsEnabledAlpha", &ErectusIni::npcSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled NPCs", &ErectusIni::npcSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsDisabledAlpha", &ErectusIni::npcSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named NPCs", &ErectusIni::npcSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed NPCs", &ErectusIni::npcSettings.drawUnnamed);

					ButtonToggle("Show NPC Name", &ErectusIni::npcSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show NPC Distance", &ErectusIni::npcSettings.showDistance);

					ButtonToggle("Show NPC Health", &ErectusIni::npcSettings.showHealth);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Dead NPC Health", &ErectusIni::npcSettings.showDeadHealth);

					ButtonToggle("NPC Text Shadowed", &ErectusIni::npcSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("NPC Text Centered", &ErectusIni::npcSettings.textCentered);

					ButtonToggle("Always Draw Living 1* NPCs", &ErectusIni::customLegendarySettings.overrideLivingOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingOneStarColor", ErectusIni::customLegendarySettings.livingOneStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingOneStarColor);

					ButtonToggle("Always Draw Dead 1* NPCs", &ErectusIni::customLegendarySettings.overrideDeadOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadOneStarColor", ErectusIni::customLegendarySettings.deadOneStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadOneStarColor);

					ButtonToggle("Always Draw Living 2* NPCs", &ErectusIni::customLegendarySettings.overrideLivingTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingTwoStarColor", ErectusIni::customLegendarySettings.livingTwoStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingTwoStarColor);

					ButtonToggle("Always Draw Dead 2* NPCs", &ErectusIni::customLegendarySettings.overrideDeadTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadTwoStarColor", ErectusIni::customLegendarySettings.deadTwoStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadTwoStarColor);

					ButtonToggle("Always Draw Living 3* NPCs", &ErectusIni::customLegendarySettings.overrideLivingThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingThreeStarColor", ErectusIni::customLegendarySettings.livingThreeStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingThreeStarColor);

					ButtonToggle("Always Draw Dead 3* NPCs", &ErectusIni::customLegendarySettings.overrideDeadThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadThreeStarColor", ErectusIni::customLegendarySettings.deadThreeStarColor);
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadThreeStarColor);

					LargeButtonToggle("Hide NPCs in the Settler Faction", &ErectusIni::customExtraNpcSettings.hideSettlerFaction);
					LargeButtonToggle("Hide NPCs in the Crater Raider Faction", &ErectusIni::customExtraNpcSettings.hideCraterRaiderFaction);
					LargeButtonToggle("Hide NPCs in the Diehards Faction", &ErectusIni::customExtraNpcSettings.hideDieHardFaction);
					LargeButtonToggle("Hide NPCs in the Secret Service Faction", &ErectusIni::customExtraNpcSettings.hideSecretServiceFaction);

					LargeButtonToggle("NPC Blacklist Enabled", &ErectusIni::customExtraNpcSettings.useNpcBlacklist);
					if (ImGui::CollapsingHeader("NPC Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							auto toggleLabel = fmt::format("NPC Blacklist: {:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::customExtraNpcSettings.npcBlacklistEnabled[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###NPCBlacklist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::customExtraNpcSettings.npcBlacklist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container ESP Settings"))
				{
					ButtonToggle("Container ESP Enabled", &ErectusIni::containerSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ContainerSettingsEnabledDistance", &ErectusIni::containerSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ContainerSettingsColor", ErectusIni::containerSettings.color);
					Utils::ValidateRgb(ErectusIni::containerSettings.color);

					ButtonToggle("Draw Enabled Containers", &ErectusIni::containerSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsEnabledAlpha", &ErectusIni::containerSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Containers", &ErectusIni::containerSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsDisabledAlpha", &ErectusIni::containerSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Containers", &ErectusIni::containerSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Containers", &ErectusIni::containerSettings.drawUnnamed);

					ButtonToggle("Show Container Name", &ErectusIni::containerSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Container Distance", &ErectusIni::containerSettings.showDistance);

					ButtonToggle("Container Text Shadowed", &ErectusIni::containerSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Container Text Centered", &ErectusIni::containerSettings.textCentered);

					if (ImGui::CollapsingHeader("Container Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Container Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::containerSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ContainerWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::containerSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Junk ESP Settings"))
				{
					ButtonToggle("Junk ESP Enabled", &ErectusIni::junkSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###JunkSettingsEnabledDistance", &ErectusIni::junkSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###JunkSettingsColor", ErectusIni::junkSettings.color);
					Utils::ValidateRgb(ErectusIni::junkSettings.color);

					ButtonToggle("Draw Enabled Junk", &ErectusIni::junkSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsEnabledAlpha", &ErectusIni::junkSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					Utils::ValidateFloat(&ErectusIni::junkSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Junk", &ErectusIni::junkSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsDisabledAlpha", &ErectusIni::junkSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Junk", &ErectusIni::junkSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Junk", &ErectusIni::junkSettings.drawUnnamed);

					ButtonToggle("Show Junk Name", &ErectusIni::junkSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Junk Distance", &ErectusIni::junkSettings.showDistance);

					ButtonToggle("Junk Text Shadowed", &ErectusIni::junkSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Text Centered", &ErectusIni::junkSettings.textCentered);

					if (ImGui::CollapsingHeader("Junk Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Junk Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::junkSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###JunkWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::junkSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Plan ESP Settings"))
				{
					ButtonToggle("Plan ESP Enabled", &ErectusIni::planSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###PlanSettingsEnabledDistance", &ErectusIni::planSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###PlanSettingsColor", ErectusIni::planSettings.color);
					Utils::ValidateRgb(ErectusIni::planSettings.color);

					ButtonToggle("Draw Enabled Plans", &ErectusIni::planSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsEnabledAlpha", &ErectusIni::planSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Plans", &ErectusIni::planSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsDisabledAlpha", &ErectusIni::planSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Known Plans", &ErectusIni::customKnownRecipeSettings.knownRecipesEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unknown Plans", &ErectusIni::customKnownRecipeSettings.unknownRecipesEnabled);

					ButtonToggle("Draw Named Plans", &ErectusIni::planSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Plans", &ErectusIni::planSettings.drawUnnamed);

					ButtonToggle("Show Plan Name", &ErectusIni::planSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Plan Distance", &ErectusIni::planSettings.showDistance);

					ButtonToggle("Plan Text Shadowed", &ErectusIni::planSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Plan Text Centered", &ErectusIni::planSettings.textCentered);

					if (ImGui::CollapsingHeader("Plan Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Plan Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::planSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###PlanWhitelis{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::planSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Magazine ESP Settings"))
				{
					ButtonToggle("Magazine ESP Enabled", &ErectusIni::magazineSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###MagazineSettingsEnabledDistance", &ErectusIni::magazineSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###MagazineSettingsColor", ErectusIni::magazineSettings.color);
					Utils::ValidateRgb(ErectusIni::magazineSettings.color);

					ButtonToggle("Draw Enabled Magazines", &ErectusIni::magazineSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsEnabledAlpha", &ErectusIni::magazineSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Magazines", &ErectusIni::magazineSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsDisabledAlpha", &ErectusIni::magazineSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Magazines", &ErectusIni::magazineSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Magazines", &ErectusIni::magazineSettings.drawUnnamed);

					ButtonToggle("Show Magazine Name", &ErectusIni::magazineSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Magazine Distance", &ErectusIni::magazineSettings.showDistance);

					ButtonToggle("Magazine Text Shadowed", &ErectusIni::magazineSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Magazine Text Centered", &ErectusIni::magazineSettings.textCentered);

					if (ImGui::CollapsingHeader("Magazine Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Magazine Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::magazineSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###MagazineWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::magazineSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Bobblehead ESP Settings"))
				{
					ButtonToggle("Bobblehead ESP Enabled", &ErectusIni::bobbleheadSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###BobbleheadSettingsEnabledDistance", &ErectusIni::bobbleheadSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###BobbleheadSettingsColor", ErectusIni::bobbleheadSettings.color);
					Utils::ValidateRgb(ErectusIni::bobbleheadSettings.color);

					ButtonToggle("Draw Enabled Bobbleheads", &ErectusIni::bobbleheadSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsEnabledAlpha", &ErectusIni::bobbleheadSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Bobbleheads", &ErectusIni::bobbleheadSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsDisabledAlpha", &ErectusIni::bobbleheadSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Bobbleheads", &ErectusIni::bobbleheadSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Bobbleheads", &ErectusIni::bobbleheadSettings.drawUnnamed);

					ButtonToggle("Show Bobblehead Name", &ErectusIni::bobbleheadSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Bobblehead Distance", &ErectusIni::bobbleheadSettings.showDistance);

					ButtonToggle("Bobblehead Text Shadowed", &ErectusIni::bobbleheadSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Bobblehead Text Centered", &ErectusIni::bobbleheadSettings.textCentered);

					if (ImGui::CollapsingHeader("Bobblehead Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Bobblehead Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::bobbleheadSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###BobbleheadWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::bobbleheadSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Item ESP Settings"))
				{
					ButtonToggle("Item ESP Enabled", &ErectusIni::itemSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemSettingsEnabledDistance", &ErectusIni::itemSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ItemSettingsColor", ErectusIni::itemSettings.color);
					Utils::ValidateRgb(ErectusIni::itemSettings.color);

					ButtonToggle("Draw Enabled Items", &ErectusIni::itemSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsEnabledAlpha", &ErectusIni::itemSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Items", &ErectusIni::itemSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsDisabledAlpha", &ErectusIni::itemSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Items", &ErectusIni::itemSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Items", &ErectusIni::itemSettings.drawUnnamed);

					ButtonToggle("Show Item Name", &ErectusIni::itemSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Item Distance", &ErectusIni::itemSettings.showDistance);

					ButtonToggle("Item Text Shadowed", &ErectusIni::itemSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Item Text Centered", &ErectusIni::itemSettings.textCentered);

					if (ImGui::CollapsingHeader("Item Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Item Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::itemSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ItemWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::itemSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora ESP Settings"))
				{
					ButtonToggle("Flora ESP Enabled", &ErectusIni::floraSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###FloraSettingsEnabledDistance", &ErectusIni::floraSettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###FloraSettingsColor", ErectusIni::floraSettings.color);
					Utils::ValidateRgb(ErectusIni::floraSettings.color);

					ButtonToggle("Draw Enabled Flora", &ErectusIni::floraSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsEnabledAlpha", &ErectusIni::floraSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Flora", &ErectusIni::floraSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsDisabledAlpha", &ErectusIni::floraSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Flora", &ErectusIni::floraSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Flora", &ErectusIni::floraSettings.drawUnnamed);

					LargeButtonToggle("Draw Raw Crimson Flux Yielding Flora", &ErectusIni::customFluxSettings.crimsonFluxEnabled);
					LargeButtonToggle("Draw Raw Cobalt Flux Yielding Flora", &ErectusIni::customFluxSettings.cobaltFluxEnabled);
					LargeButtonToggle("Draw Raw Yellowcake Flux Yielding Flora", &ErectusIni::customFluxSettings.yellowcakeFluxEnabled);
					LargeButtonToggle("Draw Raw Fluorescent Flux Yielding Flora", &ErectusIni::customFluxSettings.fluorescentFluxEnabled);
					LargeButtonToggle("Draw Raw Violet Flux Yielding Flora", &ErectusIni::customFluxSettings.violetFluxEnabled);

					ButtonToggle("Show Flora Name", &ErectusIni::floraSettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Flora Distance", &ErectusIni::floraSettings.showDistance);

					ButtonToggle("Flora Text Shadowed", &ErectusIni::floraSettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Flora Text Centered", &ErectusIni::floraSettings.textCentered);

					if (ImGui::CollapsingHeader("Flora Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Flora Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::floraSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###FloraWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::floraSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Entity ESP Settings"))
				{
					ButtonToggle("Entity ESP Enabled", &ErectusIni::entitySettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###EntitySettingsEnabledDistance", &ErectusIni::entitySettings.enabledDistance, 0, 3000, "Distance: %d");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###EntitySettingsColor", ErectusIni::entitySettings.color);
					Utils::ValidateRgb(ErectusIni::entitySettings.color);

					ButtonToggle("Draw Enabled Entities", &ErectusIni::entitySettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsEnabledAlpha", &ErectusIni::entitySettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Disabled Entities", &ErectusIni::entitySettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsDisabledAlpha", &ErectusIni::entitySettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

					ButtonToggle("Draw Named Entities", &ErectusIni::entitySettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Entities", &ErectusIni::entitySettings.drawUnnamed);

					ButtonToggle("Show Entity Name", &ErectusIni::entitySettings.showName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Entity Distance", &ErectusIni::entitySettings.showDistance);

					ButtonToggle("Entity Text Shadowed", &ErectusIni::entitySettings.textShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Entity Text Centered", &ErectusIni::entitySettings.textCentered);

					if (ImGui::CollapsingHeader("Entity Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Entity Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::floraSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###EntityWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::entitySettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Loot###LootTab"))
			{
				if (ImGui::CollapsingHeader("Scrap Looter"))
				{
					if (ErectusMemory::CheckScrapList())
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Loot Scrap (Keybind: CTRL+E)###LootSelectedScrapEnabled", ImVec2(224.0f, 0.0f)))
							ErectusMemory::LootScrap();
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Loot Scrap (Keybind: CTRL+E)###LootSelectedScrapDisabled", ImVec2(224.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}

					ImGui::SameLine(235.0f);
					ButtonToggle("Scrap Looter Keybind Enabled", &ErectusIni::scrapLooter.keybindEnabled);

					LargeButtonToggle("Scrap Looter ESP Override (Uses Junk ESP Settings)", &ErectusIni::scrapLooter.scrapOverrideEnabled);

					ButtonToggle("Automatic Looting Enabled###ScrapAutomaticLootingEnabled", &ErectusIni::scrapLooter.autoLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ScrapAutomaticStatus", &ErectusIni::scrapLooter.drawStatus);

					ImGui::SetNextItemWidth(224.0f);
					auto sliderText = fmt::format("Speed (Min): {0:d} ({1:d} ms)", ErectusIni::scrapLooter.autoLootingSpeedMin, ErectusIni::scrapLooter.autoLootingSpeedMin * 16);
					if(ImGui::SliderInt("###ScrapAutomaticSpeedMin", &ErectusIni::scrapLooter.autoLootingSpeedMin, 10, 60, sliderText.c_str()))
					{
						if (ErectusIni::scrapLooter.autoLootingSpeedMax < ErectusIni::scrapLooter.autoLootingSpeedMin)
							ErectusIni::scrapLooter.autoLootingSpeedMax = ErectusIni::scrapLooter.autoLootingSpeedMin;
					}

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					
					sliderText = fmt::format("Speed (Max): {0:d} ({1:d} ms)", ErectusIni::scrapLooter.autoLootingSpeedMax, ErectusIni::scrapLooter.autoLootingSpeedMax * 16);
					if(ImGui::SliderInt("###ScrapAutomaticSpeedMax", &ErectusIni::scrapLooter.autoLootingSpeedMax, 10, 60, sliderText.c_str()))
					{
						if (ErectusIni::scrapLooter.autoLootingSpeedMax < ErectusIni::scrapLooter.autoLootingSpeedMin)
							ErectusIni::scrapLooter.autoLootingSpeedMin = ErectusIni::scrapLooter.autoLootingSpeedMax;
					}

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderInt("###ScrapLooterDistance", &ErectusIni::scrapLooter.maxDistance, 1, 3000, "Scrap Looter Distance: %d");

					for (auto i = 0; i < 40; i++)
					{
						ButtonToggle(ErectusIni::scrapLooter.nameList[i], &ErectusIni::scrapLooter.enabledList[i]);

						ImGui::SameLine(235.0f);
						ImGui::SetNextItemWidth(224.0f);

						auto inputLabel = fmt::format("###ScrapReadOnly{:d}", i);
						auto inputText = fmt::format("{:08X}", ErectusIni::scrapLooter.formIdList[i]);
						ImGui::InputText(inputLabel.c_str(), &inputText, ImGuiInputTextFlags_ReadOnly);
					}
				}

				if (ImGui::CollapsingHeader("Item Looter"))
				{
					if (ErectusMemory::CheckItemLooterSettings())
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Loot Items (Keybind: CTRL+R)###LootSelectedItemsEnabled", ImVec2(224.0f, 0.0f)))
							ErectusMemory::LootItems();
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Loot Items (Keybind: CTRL+R)###LootSelectedItemsDisabled", ImVec2(224.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}

					ImGui::SameLine(235.0f);
					ButtonToggle("Item Looter Keybind Enabled", &ErectusIni::itemLooter.keybindEnabled);

					ButtonToggle("Automatic Looting Enabled###ItemAutomaticLootingEnabled", &ErectusIni::itemLooter.autoLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ItemAutomaticStatus", &ErectusIni::itemLooter.drawStatus);

					ImGui::SetNextItemWidth(224.0f);
					char itemAutomaticSpeedMinText[sizeof"Speed (Min): 60 (960 ms)"];
					sprintf_s(itemAutomaticSpeedMinText, "Speed (Min): %d (%d ms)",
						ErectusIni::itemLooter.autoLootingSpeedMin,
						ErectusIni::itemLooter.autoLootingSpeedMin * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMin", &ErectusIni::itemLooter.autoLootingSpeedMin, 10, 60, itemAutomaticSpeedMinText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::itemLooter.autoLootingSpeedMax < ErectusIni::itemLooter.autoLootingSpeedMin)
							ErectusIni::itemLooter.autoLootingSpeedMax = ErectusIni::itemLooter.autoLootingSpeedMin;
					}

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					char itemAutomaticSpeedMaxText[sizeof"Speed (Max): 60 (960 ms)"];
					sprintf_s(itemAutomaticSpeedMaxText, "Speed (Max): %d (%d ms)",
						ErectusIni::itemLooter.autoLootingSpeedMax,
						ErectusIni::itemLooter.autoLootingSpeedMax * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMax", &ErectusIni::itemLooter.autoLootingSpeedMax, 10, 60, itemAutomaticSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::itemLooter.autoLootingSpeedMax < ErectusIni::itemLooter.autoLootingSpeedMin)
							ErectusIni::itemLooter.autoLootingSpeedMin = ErectusIni::itemLooter.autoLootingSpeedMax;
					}

					if (ErectusIni::itemLooter.autoLootingSpeedMax < ErectusIni::itemLooter.autoLootingSpeedMin)
						ErectusIni::itemLooter.autoLootingSpeedMax = ErectusIni::itemLooter.autoLootingSpeedMin;

					ButtonToggle("Weapons Enabled###ItemLooterWeaponsEnabled", &ErectusIni::itemLooter.lootWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterWeaponsDistance", &ErectusIni::itemLooter.lootWeaponsDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Armor Enabled###ItemLooterArmorEnabled", &ErectusIni::itemLooter.lootArmorEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterArmorDistance", &ErectusIni::itemLooter.lootArmorDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Ammo Enabled###ItemLooterAmmoEnabled", &ErectusIni::itemLooter.lootAmmoEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAmmoDistance", &ErectusIni::itemLooter.lootAmmoDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Mods Enabled###ItemLooterModsEnabled", &ErectusIni::itemLooter.lootModsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterModsDistance", &ErectusIni::itemLooter.lootModsDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Magazines Enabled###ItemLooterMagazinesEnabled", &ErectusIni::itemLooter.lootMagazinesEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMagazinesDistance", &ErectusIni::itemLooter.lootMagazinesDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Bobbleheads Enabled###ItemLooterBobbleheadsEnabled", &ErectusIni::itemLooter.lootBobbleheadsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterBobbleheadsDistance", &ErectusIni::itemLooter.lootBobbleheadsDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Aid Enabled###ItemLooterAidEnabled", &ErectusIni::itemLooter.lootAidEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAidDistance", &ErectusIni::itemLooter.lootAidDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Known Plans Enabled###ItemLooterKnownPlansEnabled", &ErectusIni::itemLooter.lootKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterKnownPlansDistance", &ErectusIni::itemLooter.lootKnownPlansDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Unknown Plans Enabled###ItemLooterUnknownPlansEnabled", &ErectusIni::itemLooter.lootUnknownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnknownPlansDistance", &ErectusIni::itemLooter.lootUnknownPlansDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Misc Enabled###ItemLooterMiscEnabled", &ErectusIni::itemLooter.lootMiscEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMiscDistance", &ErectusIni::itemLooter.lootMiscDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Other Enabled###ItemLooterUnlistedEnabled", &ErectusIni::itemLooter.lootUnlistedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnlistedDistance", &ErectusIni::itemLooter.lootUnlistedDistance, 0, 3000, "Distance: %d");

					ButtonToggle("Item FormId List Enabled###ItemLooterListEnabled", &ErectusIni::itemLooter.lootListEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterListDistance", &ErectusIni::itemLooter.lootListDistance, 0, 3000, "Distance: %d");

					LargeButtonToggle("Item Looter Blacklist Enabled###ItemLooterBlacklistToggle", &ErectusIni::itemLooter.blacklistToggle);

					if (ImGui::CollapsingHeader("Item Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							auto toggleLabel = fmt::format("Item Looter Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::itemLooter.enabledList[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ItemLooterList{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::itemLooter.formIdList[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}

					if (ImGui::CollapsingHeader("Item Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							auto toggleLabel = fmt::format("Item Looter Blacklist: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::itemLooter.blacklistEnabled[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ItemLooterBlacklist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::itemLooter.blacklist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("NPC Looter (76m Distance Limit)"))
				{
					LargeButtonToggle("Automatic NPC Looting Enabled (Keybind: CTRL+COMMA)###NPCLooterEnabled", &ErectusIni::npcLooter.enabled);

					LargeButtonToggle("Draw NPC Looter Status###NPCLooterStatusEnabled", &ErectusIni::npcLooter.drawStatus);

					ButtonToggle("All Weapons Enabled###NPCLooterAllWeaponsEnabled", &ErectusIni::npcLooter.allWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###NPCLooterAllArmorEnabled", &ErectusIni::npcLooter.allArmorEnabled);

					ButtonToggle("1* Weapons Enabled###NPCLooterOneStarWeaponsEnabled", &ErectusIni::npcLooter.oneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###NPCLooterOneStarArmorEnabled", &ErectusIni::npcLooter.oneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###NPCLooterTwoStarWeaponsEnabled", &ErectusIni::npcLooter.twoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###NPCLooterTwoStarArmorEnabled", &ErectusIni::npcLooter.twoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###NPCLooterThreeStarWeaponsEnabled", &ErectusIni::npcLooter.threeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###NPCLooterThreeStarArmorEnabled", &ErectusIni::npcLooter.threeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###NPCLooterAmmoEnabled", &ErectusIni::npcLooter.ammoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###NPCLooterModsEnabled", &ErectusIni::npcLooter.modsEnabled);

					ButtonToggle("Caps Enabled###NPCLooterCapsEnabled", &ErectusIni::npcLooter.capsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###NPCLooterJunkEnabled", &ErectusIni::npcLooter.junkEnabled);

					ButtonToggle("Aid Enabled###NPCLooterAidEnabled", &ErectusIni::npcLooter.aidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###NPCLooterTreasureMapsEnabled", &ErectusIni::npcLooter.treasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###NPCLooterKnownPlansEnabled", &ErectusIni::npcLooter.knownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###NPCLooterUnknownPlansEnabled", &ErectusIni::npcLooter.unknownPlansEnabled);

					ButtonToggle("Misc Enabled###NPCLooterMiscEnabled", &ErectusIni::npcLooter.miscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###NPCLooterUnlistedEnabled", &ErectusIni::npcLooter.unlistedEnabled);

					LargeButtonToggle("NPC Looter FormId List Enabled###NPCLooterListEnabled", &ErectusIni::npcLooter.listEnabled);
					LargeButtonToggle("NPC Looter Blacklist Enabled###NPCLooterBlacklistToggle", &ErectusIni::npcLooter.blacklistToggle);

					if (ImGui::CollapsingHeader("NPC Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							auto toggleLabel = fmt::format("NPC Looter Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::npcLooter.enabledList[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###NPCLooterList{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::npcLooter.formIdList[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}

					if (ImGui::CollapsingHeader("NPC Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							auto toggleLabel = fmt::format("NPC Looter Blacklist: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::npcLooter.blacklistEnabled[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###NPCLooterBlacklist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::npcLooter.blacklist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container Looter (6m Distance Limit)"))
				{
					LargeButtonToggle("Automatic Container Looting Enabled (Keybind: CTRL+PERIOD)###ContainerLooterEnabled", &ErectusIni::containerLooter.enabled);

					LargeButtonToggle("Draw Container Looter Status###ContainerLooterStatusEnabled", &ErectusIni::containerLooter.drawStatus);

					ButtonToggle("All Weapons Enabled###ContainerLooterAllWeaponsEnabled", &ErectusIni::containerLooter.allWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###ContainerLooterAllArmorEnabled", &ErectusIni::containerLooter.allArmorEnabled);

					ButtonToggle("1* Weapons Enabled###ContainerLooterOneStarWeaponsEnabled", &ErectusIni::containerLooter.oneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###ContainerLooterOneStarArmorEnabled", &ErectusIni::containerLooter.oneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###ContainerLooterTwoStarWeaponsEnabled", &ErectusIni::containerLooter.twoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###ContainerLooterTwoStarArmorEnabled", &ErectusIni::containerLooter.twoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###ContainerLooterThreeStarWeaponsEnabled", &ErectusIni::containerLooter.threeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###ContainerLooterThreeStarArmorEnabled", &ErectusIni::containerLooter.threeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###ContainerLooterAmmoEnabled", &ErectusIni::containerLooter.ammoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###ContainerLooterModsEnabled", &ErectusIni::containerLooter.modsEnabled);

					ButtonToggle("Caps Enabled###ContainerLooterCapsEnabled", &ErectusIni::containerLooter.capsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###ContainerLooterJunkEnabled", &ErectusIni::containerLooter.junkEnabled);

					ButtonToggle("Aid Enabled###ContainerLooterAidEnabled", &ErectusIni::containerLooter.aidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###ContainerLooterTreasureMapsEnabled", &ErectusIni::containerLooter.treasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###ContainerLooterKnownPlansEnabled", &ErectusIni::containerLooter.knownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###ContainerLooterUnknownPlansEnabled", &ErectusIni::containerLooter.unknownPlansEnabled);

					ButtonToggle("Misc Enabled###ContainerLooterMiscEnabled", &ErectusIni::containerLooter.miscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###ContainerLooterUnlistedEnabled", &ErectusIni::containerLooter.unlistedEnabled);

					LargeButtonToggle("Container Looter FormId List Enabled###ContainerLooterListEnabled", &ErectusIni::containerLooter.listEnabled);

					LargeButtonToggle("Container Looter Blacklist Enabled###ContainerLooterBlacklistToggle", &ErectusIni::containerLooter.blacklistToggle);

					if (ImGui::CollapsingHeader("Container Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							auto toggleLabel = fmt::format("Container Looter Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::containerLooter.enabledList[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ContainerLooterList{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::containerLooter.formIdList[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}

					if (ImGui::CollapsingHeader("Container Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							auto toggleLabel = fmt::format("Container Looter Blacklist: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::containerLooter.blacklistEnabled[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ContainerLooterBlacklist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::containerLooter.blacklist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora Harvester (6m Distance Limit)"))
				{
					LargeButtonToggle("Automatic Flora Harvesting Enabled (Keybind: CTRL+P])###HarvesterEnabled", &ErectusIni::harvester.enabled);
					LargeButtonToggle("Draw Flora Harvester Status###HarvesterStatusEnabled", &ErectusIni::harvester.drawStatus);
					LargeButtonToggle("Flora Harvester ESP Override (Uses Flora ESP Settings)", &ErectusIni::harvester.overrideEnabled);

					for (auto i = 0; i < 69; i++)
					{
						ButtonToggle(ErectusIni::harvester.nameList[i], &ErectusIni::harvester.enabledList[i]);

						ImGui::SameLine(235.0f);
						ImGui::SetNextItemWidth(224.0f);

						auto inputLabel = fmt::format("###HarvesterReadOnly{:d}", i);
						auto inputText = fmt::format("{:08X}", ErectusIni::harvester.formIdList[i]);
						ImGui::InputText(inputLabel.c_str(), &inputText, ImGuiInputTextFlags_ReadOnly);
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Combat###CombatTab"))
			{
				if (ImGui::CollapsingHeader("Weapon Editor"))
				{
					ButtonToggle("No Recoil", &ErectusIni::customWeaponSettings.noRecoil);
					ImGui::SameLine(235.0f);
					ButtonToggle("Infinite Ammo", &ErectusIni::customWeaponSettings.infiniteAmmo);

					ButtonToggle("No Spread", &ErectusIni::customWeaponSettings.noSpread);
					ImGui::SameLine(235.0f);
					ButtonToggle("Instant Reload", &ErectusIni::customWeaponSettings.instantReload);

					ButtonToggle("No Sway", &ErectusIni::customWeaponSettings.noSway);
					ImGui::SameLine(235.0f);
					ButtonToggle("Automatic Flag###WeaponAutomatic", &ErectusIni::customWeaponSettings.automaticflag);

					ButtonToggle("Capacity###WeaponCapacityEnabled", &ErectusIni::customWeaponSettings.capacityEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###WeaponCapacity", &ErectusIni::customWeaponSettings.capacity, 0, 999, "Capacity: %d");

					ButtonToggle("Speed###WeaponSpeedEnabled", &ErectusIni::customWeaponSettings.speedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponSpeed", &ErectusIni::customWeaponSettings.speed, 0.0f, 100.0f, "Speed: %.2f");

					ButtonToggle("Reach###WeaponReachEnabled", &ErectusIni::customWeaponSettings.reachEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponReach", &ErectusIni::customWeaponSettings.reach, 0.0f, 999.0f, "Reach: %.2f");
				}

				if (ImGui::CollapsingHeader("Targeting Settings"))
				{
					ButtonToggle("Player Targeting (Keybind: T)", &ErectusIni::customTargetSettings.lockPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("NPC Targeting (Keybind: T)", &ErectusIni::customTargetSettings.lockNpCs);

					ButtonToggle("Damage Redirection (Players)", &ErectusIni::customTargetSettings.indirectPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("Damage Redirection (NPCs)", &ErectusIni::customTargetSettings.indirectNpCs);

					ButtonToggle("Send Damage (Players)", &ErectusIni::customTargetSettings.directPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("Send Damage (NPCs)", &ErectusIni::customTargetSettings.directNpCs);

					SmallButtonToggle("Living###TargetLiving", &ErectusIni::customTargetSettings.targetLiving);
					ImGui::SameLine(122.0f);
					SmallButtonToggle("Downed###TargetDowned", &ErectusIni::customTargetSettings.targetDowned);
					ImGui::SameLine(235.0f);
					SmallButtonToggle("Dead###TargetDead", &ErectusIni::customTargetSettings.targetDead);
					ImGui::SameLine(349.0f);
					SmallButtonToggle("Unknown###TargetUnknown", &ErectusIni::customTargetSettings.targetUnknown);

					ButtonToggle("Ignore Render Distance###IgnoreRenderDistance", &ErectusIni::customTargetSettings.ignoreRenderDistance);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###TargetLockingFOV", &ErectusIni::customTargetSettings.lockingFov, 5.0f, 40.0f, "Targeting FOV: %.2f");

					ButtonToggle("Ignore Essential NPCs###IgnoreEssentialNPCs", &ErectusIni::customTargetSettings.ignoreEssentialNpCs);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###TargetLockingColor", ErectusIni::customTargetSettings.lockingColor);
					Utils::ValidateRgb(ErectusIni::playerSettings.unknownColor);

					ButtonToggle("Automatic Retargeting###TargetLockingRetargeting", &ErectusIni::customTargetSettings.retargeting);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					char targetLockingCooldownText[sizeof"Cooldown: 120 (1920 ms)"];
					sprintf_s(targetLockingCooldownText, "Cooldown: %d (%d ms)",
						ErectusIni::customTargetSettings.cooldown,
						ErectusIni::customTargetSettings.cooldown * 16);
					ImGui::SliderInt("###TargetLockingCooldown", &ErectusIni::customTargetSettings.cooldown, 0, 120, targetLockingCooldownText);

					ImGui::SetNextItemWidth(224.0f);

					char sendDamageMinText[sizeof"Send Damage (Min): 60 (960 ms)"];
					sprintf_s(sendDamageMinText, "Send Damage (Min): %d (%d ms)",
						ErectusIni::customTargetSettings.sendDamageMin,
						ErectusIni::customTargetSettings.sendDamageMin * 16);
					ImGui::SliderInt("###SendDamageMin", &ErectusIni::customTargetSettings.sendDamageMin, 1, 60, sendDamageMinText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.sendDamageMin)
							ErectusIni::customTargetSettings.sendDamageMax = ErectusIni::customTargetSettings.sendDamageMin;
					}

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					char sendDamageMaxText[sizeof"Send Damage (Max): 60 (960 ms)"];
					sprintf_s(sendDamageMaxText, "Send Damage (Max): %d (%d ms)",
						ErectusIni::customTargetSettings.sendDamageMax,
						ErectusIni::customTargetSettings.sendDamageMax * 16);
					ImGui::SliderInt("###SendDamageMax", &ErectusIni::customTargetSettings.sendDamageMax, 1, 60, sendDamageMaxText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.sendDamageMin)
							ErectusIni::customTargetSettings.sendDamageMin = ErectusIni::customTargetSettings.sendDamageMax;
					}

					if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.sendDamageMin)
						ErectusIni::customTargetSettings.sendDamageMax = ErectusIni::customTargetSettings.sendDamageMin;

					std::string favoritedWeaponsPreview = "[?] No Weapon Selected";
					if (ErectusIni::customTargetSettings.favoriteIndex < 12)
					{
						favoritedWeaponsPreview = ErectusMemory::GetFavoritedWeaponText(BYTE(ErectusIni::customTargetSettings.favoriteIndex));
						if (favoritedWeaponsPreview.empty())
						{
							favoritedWeaponsPreview = fmt::format("[{}] Favorited Item Invalid", ErectusMemory::FavoriteIndex2Slot(BYTE(ErectusIni::customTargetSettings.favoriteIndex)));
						}
					}

					ImGui::SetNextItemWidth(451.0f);
					if (ImGui::BeginCombo("###BeginTempCombo", favoritedWeaponsPreview.c_str()))
					{
						for (const auto& item : ErectusMemory::GetFavoritedWeapons())
						{
							if (ImGui::Selectable(item.second.c_str()))
							{
								if (item.first)
									ErectusIni::customTargetSettings.favoriteIndex = item.first - 1;
								else
									ErectusIni::customTargetSettings.favoriteIndex = 12;
							}
						}
						Utils::ValidateInt(&ErectusIni::customTargetSettings.favoriteIndex, 0, 12);

						ImGui::EndCombo();
					}
				}

				if (ImGui::CollapsingHeader("Melee Settings"))
				{
					LargeButtonToggle("Melee Enabled (Keybind: U)", &ErectusIni::melee.enabled);

					ImGui::SetNextItemWidth(224.0f);
					char meleeSpeedMinText[sizeof"Melee Speed (Min): 60 (960 ms)"];
					sprintf_s(meleeSpeedMinText, "Melee Speed (Min): %d (%d ms)",
						ErectusIni::melee.speedMin,
						ErectusIni::melee.speedMin * 16);
					ImGui::SliderInt("###MeleeSpeedMin", &ErectusIni::melee.speedMin, 1, 60, meleeSpeedMinText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::melee.speedMax < ErectusIni::melee.speedMin)
							ErectusIni::melee.speedMax = ErectusIni::melee.speedMin;
					}

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					char meleeSpeedMaxText[sizeof"Melee Speed (Max): 60 (960 ms)"];
					sprintf_s(meleeSpeedMaxText, "Melee Speed (Max): %d (%d ms)",
						ErectusIni::melee.speedMax,
						ErectusIni::melee.speedMax * 16);
					ImGui::SliderInt("###MeleeSpeedMax", &ErectusIni::melee.speedMax, 1, 60, meleeSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						if (ErectusIni::melee.speedMax < ErectusIni::melee.speedMin)
							ErectusIni::melee.speedMin = ErectusIni::melee.speedMax;
					}

					if (ErectusIni::melee.speedMax < ErectusIni::melee.speedMin)
						ErectusIni::melee.speedMax = ErectusIni::melee.speedMin;
				}

				if (ImGui::CollapsingHeader("One Position Kill"))
				{
					LargeButtonToggle("OPK Players (Keybind: CTRL+B)", &ErectusIni::customOpkSettings.playersEnabled);
					LargeButtonToggle("OPK NPCs (Keybind: CTRL+N)", &ErectusIni::customOpkSettings.npcsEnabled);
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Player###PlayerTab"))
			{
				if (ImGui::CollapsingHeader("Local Player Settings"))
				{
					LargeButtonToggle("Position Spoofing (Keybind CTRL+L)##LocalPlayerPositionSpoofingEnabled", &ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled);
					ButtonToggle("Draw Position Status###LocalPlayerDrawPositionSpoofingEnabled", &ErectusIni::customLocalPlayerSettings.drawPositionSpoofingStatus);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerPositionSpoofingHeight", &ErectusIni::customLocalPlayerSettings.positionSpoofingHeight, -524287, 524287, "Spoofed Height: %d");

					ButtonToggle("Noclip (Keybind CTRL+Y)###NoclipEnabled", &ErectusIni::customLocalPlayerSettings.noclipEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderFloat("###NoclipSpeed", &ErectusIni::customLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f, "Speed: %.5f");

					ButtonToggle("Client State", &ErectusIni::customLocalPlayerSettings.clientState);
					ImGui::SameLine(235.0f);
					ButtonToggle("Automatic Client State", &ErectusIni::customLocalPlayerSettings.automaticClientState);

					LargeButtonToggle("Freeze Action Points###LocalPlayerFreezeApEnabled", &ErectusIni::customLocalPlayerSettings.freezeApEnabled);

					ButtonToggle("Action Points###LocalPlayerAPEnabled", &ErectusIni::customLocalPlayerSettings.actionPointsEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerAP", &ErectusIni::customLocalPlayerSettings.actionPoints, 0, 99999, "Action Points: %d");

					ButtonToggle("Strength###LocalPlayerStrengthEnabled", &ErectusIni::customLocalPlayerSettings.strengthEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerStrength", &ErectusIni::customLocalPlayerSettings.strength, 0, 99999, "Strength: %d");

					ButtonToggle("Perception###LocalPlayerPerceptionEnabled", &ErectusIni::customLocalPlayerSettings.perceptionEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerPerception", &ErectusIni::customLocalPlayerSettings.perception, 0, 99999, "Perception: %d");

					ButtonToggle("Endurance###LocalPlayerEnduranceEnabled", &ErectusIni::customLocalPlayerSettings.enduranceEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerEndurance", &ErectusIni::customLocalPlayerSettings.endurance, 0, 99999, "Endurance: %d");

					ButtonToggle("Charisma###LocalPlayerCharismaEnabled", &ErectusIni::customLocalPlayerSettings.charismaEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerCharisma", &ErectusIni::customLocalPlayerSettings.charisma, 0, 99999, "Charisma: %d");

					ButtonToggle("Intelligence###LocalPlayerIntelligenceEnabled", &ErectusIni::customLocalPlayerSettings.intelligenceEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerIntelligence", &ErectusIni::customLocalPlayerSettings.intelligence, 0, 99999, "Intelligence: %d");

					ButtonToggle("Agility###LocalPlayerAgilityEnabled", &ErectusIni::customLocalPlayerSettings.agilityEnabled);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					ImGui::SliderInt("###LocalPlayerAgility", &ErectusIni::customLocalPlayerSettings.agility, 0, 99999, "Agility: %d");

					ButtonToggle("Luck###LocalPlayerLuckEnabled", &ErectusIni::customLocalPlayerSettings.luckEnabled);					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerLuck", &ErectusIni::customLocalPlayerSettings.luck, 0, 99999, "Luck: %d");
				}

				if (ImGui::CollapsingHeader("Character Settings"))
				{
					LargeButtonToggle("Character Appearance Editing Enabled###ChargenEditingEnabled", &ErectusIni::customChargenSettings.chargenEditingEnabled);
					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenThin", &ErectusIni::customChargenSettings.thin, 0.0f, 1.0f, "Character Appearance (Thin): %f");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenMuscular", &ErectusIni::customChargenSettings.muscular, 0.0f, 1.0f, "Character Appearance (Muscular): %f");

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenLarge", &ErectusIni::customChargenSettings.large, 0.0f, 1.0f, "Character Appearance (Large): %f");
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Utility###UtilityTab"))
			{
				if (ImGui::CollapsingHeader("Utility"))
				{
					ButtonToggle("Draw Local Player Data", &ErectusIni::customUtilitySettings.debugPlayer);
					ImGui::SameLine(235.0f);
					ButtonToggle("ESP Debug Mode", &ErectusIni::customUtilitySettings.debugEsp);

					if (ErectusIni::customUtilitySettings.ptrFormId)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Get Pointer###GetPointerEnabled", ImVec2(224.0f, 0.0f)))
							getPtrResult = ErectusMemory::GetPtr(ErectusIni::customUtilitySettings.ptrFormId);
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Get Pointer###GetPointerDisabled", ImVec2(224.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}

					ImGui::SameLine(235.0f);
					char ptrFormIdText[sizeof"00000000"];
					sprintf_s(ptrFormIdText, "%08lX", ErectusIni::customUtilitySettings.ptrFormId);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###PtrFormIdText", ptrFormIdText, sizeof ptrFormIdText, ImGuiInputTextFlags_CharsHexadecimal))
						getPtrResult = 0;
					sscanf_s(ptrFormIdText, "%08lX", &ErectusIni::customUtilitySettings.ptrFormId);

					ImGui::SameLine(318.0f);
					char ptrPointerText[sizeof"0000000000000000"];
					sprintf_s(ptrPointerText, "%016llX", getPtrResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###PtrPointerText", ptrPointerText, sizeof ptrPointerText, ImGuiInputTextFlags_ReadOnly);

					if (ErectusIni::customUtilitySettings.addressFormId)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Get Address###GetAddressEnabled", ImVec2(224.0f, 0.0f)))
							getAddressResult = ErectusMemory::GetAddress(ErectusIni::customUtilitySettings.addressFormId);
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Get Address###GetAddressDisabled", ImVec2(224.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}

					ImGui::SameLine(235.0f);
					char addressFormIdText[sizeof"00000000"];
					sprintf_s(addressFormIdText, "%08lX", ErectusIni::customUtilitySettings.addressFormId);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###AddressFormIdText", addressFormIdText, sizeof addressFormIdText, ImGuiInputTextFlags_CharsHexadecimal))
						getAddressResult = 0;
					sscanf_s(addressFormIdText, "%08lX", &ErectusIni::customUtilitySettings.addressFormId);

					ImGui::SameLine(318.0f);
					char addressPointerText[sizeof"0000000000000000"];
					sprintf_s(addressPointerText, "%016llX", getAddressResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###AddressPointerText", addressPointerText, sizeof addressPointerText, ImGuiInputTextFlags_ReadOnly);
				}

				if (ImGui::CollapsingHeader("Reference Editor"))
				{
					ButtonToggle("Source FormId###SwapperSourceFormIdToggle", &swapperSourceToggle);
					ImGui::SameLine(235.0f);
					char sourceFormIdText[sizeof"00000000"];
					sprintf_s(sourceFormIdText, "%08lX", ErectusIni::customSwapperSettings.sourceFormId);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperSourceFormIdText", sourceFormIdText, sizeof sourceFormIdText, ImGuiInputTextFlags_CharsHexadecimal);
					sscanf_s(sourceFormIdText, "%08lX", &ErectusIni::customSwapperSettings.sourceFormId);

					ButtonToggle("Destination FormId###SwapperDestinationFormIdToggle", &swapperDestinationToggle);
					ImGui::SameLine(235.0f);
					char destinationFormIdText[sizeof"00000000"];
					sprintf_s(destinationFormIdText, "%08lX", ErectusIni::customSwapperSettings.destinationFormId);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperDestinationFormIdText", destinationFormIdText,
						sizeof destinationFormIdText, ImGuiInputTextFlags_CharsHexadecimal);
					sscanf_s(destinationFormIdText, "%08lX", &ErectusIni::customSwapperSettings.destinationFormId);

					if (swapperSourceToggle && ErectusIni::customSwapperSettings.sourceFormId && swapperDestinationToggle && ErectusIni::customSwapperSettings.destinationFormId)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceEnabled", ImVec2(451.0f, 0.0f)))
						{
							if (ErectusMemory::ReferenceSwap(ErectusIni::customSwapperSettings.sourceFormId, ErectusIni::customSwapperSettings.destinationFormId))
							{
								ErectusIni::customSwapperSettings.destinationFormId = ErectusIni::customSwapperSettings.sourceFormId;
								swapperSourceToggle = false;
								swapperDestinationToggle = false;
							}
						}
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceDisabled", ImVec2(451.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}
				}

				if (ImGui::CollapsingHeader("Item Transferring"))
				{
					SmallButtonToggle("Source###TransferSourceFormIdToggle", &transferSourceToggle);

					ImGui::SameLine(122.0f);
					char sourceFormIdText[sizeof"00000000"];
					sprintf_s(sourceFormIdText, "%08lX", ErectusIni::customTransferSettings.sourceFormId);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferSourceFormIdText", sourceFormIdText, sizeof sourceFormIdText, ImGuiInputTextFlags_CharsHexadecimal);
					sscanf_s(sourceFormIdText, "%08lX", &ErectusIni::customTransferSettings.sourceFormId);

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferSourceLocalPlayer", ImVec2(110.0f, 0.0f)))
						ErectusIni::customTransferSettings.sourceFormId = ErectusMemory::GetLocalPlayerFormId();
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferSourceSTASH", ImVec2(110.0f, 0.0f)))
						ErectusIni::customTransferSettings.sourceFormId = ErectusMemory::GetStashFormId();
					ImGui::PopStyleColor(3);

					SmallButtonToggle("Destination###TransferDestinationFormIdToggle", &transferDestinationToggle);
					ImGui::SameLine(122.0f);
					char destinationFormIdText[sizeof"00000000"];
					sprintf_s(destinationFormIdText, "%08lX", ErectusIni::customTransferSettings.destinationFormId);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferDestinationFormIdText", destinationFormIdText, sizeof destinationFormIdText, ImGuiInputTextFlags_CharsHexadecimal);
					sscanf_s(destinationFormIdText, "%08lX", &ErectusIni::customTransferSettings.destinationFormId);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferDestinationLocalPlayer", ImVec2(110.0f, 0.0f)))
						ErectusIni::customTransferSettings.destinationFormId = ErectusMemory::GetLocalPlayerFormId();
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferDestinationSTASH", ImVec2(110.0f, 0.0f)))
						ErectusIni::customTransferSettings.destinationFormId = ErectusMemory::GetStashFormId();
					ImGui::PopStyleColor(3);

					auto allowTransfer = false;

					if (transferSourceToggle && ErectusIni::customTransferSettings.sourceFormId && transferDestinationToggle && ErectusIni::customTransferSettings.destinationFormId)
					{
						if (ErectusIni::customTransferSettings.useWhitelist)
							allowTransfer = ErectusMemory::CheckItemTransferList();
						else
							allowTransfer = true;
					}

					if (allowTransfer)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Transfer Items###TransferItemsEnabled", ImVec2(451.0f, 0.0f)))
							ErectusMemory::TransferItems(ErectusIni::customTransferSettings.sourceFormId, ErectusIni::customTransferSettings.destinationFormId);
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
						ImGui::Button("Transfer Items###TransferItemsDisabled", ImVec2(451.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}

					LargeButtonToggle("Use Item Transfer Whitelist", &ErectusIni::customTransferSettings.useWhitelist);
					LargeButtonToggle("Use Item Transfer Blacklist", &ErectusIni::customTransferSettings.useBlacklist);

					if (ImGui::CollapsingHeader("Item Transfer Whitelist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Transfer Whitelist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::customTransferSettings.whitelisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ItemTransferWhitelist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::customTransferSettings.whitelist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}

					if (ImGui::CollapsingHeader("Item Transfer Blacklist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							auto toggleLabel = fmt::format("Transfer Blacklist Slot: {0:d}", i);
							ButtonToggle(toggleLabel.c_str(), &ErectusIni::customTransferSettings.blacklisted[i]);

							ImGui::SameLine(235.0f);
							ImGui::SetNextItemWidth(224.0f);

							auto inputLabel = fmt::format("###ItemTransferBlacklist{:d}", i);
							ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &ErectusIni::customTransferSettings.blacklist[i],
								nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
						}
					}
				}

				if (ImGui::CollapsingHeader("Nuke Codes"))
				{
					ButtonToggle("Automatic Nuke Codes", &ErectusIni::customNukeCodeSettings.automaticNukeCodes);
					ImGui::SameLine(235.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					if (ImGui::Button("Get Nuke Codes", ImVec2(224.0f, 0.0f)))
					{
						ErectusMemory::GetNukeCode(0x000921AE, alphaCode);
						ErectusMemory::GetNukeCode(0x00092213, bravoCode);
						ErectusMemory::GetNukeCode(0x00092214, charlieCode);
					}
					ImGui::PopStyleColor(3);

					ButtonToggle("Draw Nuke Code Alpha", &ErectusIni::customNukeCodeSettings.drawCodeAlpha);
					ImGui::SameLine(255.0f);
					auto text = fmt::format("{} - Alpha", fmt::join(alphaCode, " "));
					ImGui::Text(text.c_str());

					ButtonToggle("Draw Nuke Code Bravo", &ErectusIni::customNukeCodeSettings.drawCodeBravo);
					ImGui::SameLine(255.0f);
					text = fmt::format("{} - Bravo", fmt::join(bravoCode, " "));
					ImGui::Text(text.c_str());

					ButtonToggle("Draw Nuke Code Charlie", &ErectusIni::customNukeCodeSettings.drawCodeCharlie);
					ImGui::SameLine(255.0f);
					text = fmt::format("{} - Charlie", fmt::join(charlieCode, " "));
					ImGui::Text(text.c_str());
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Teleporter###TeleporterTab"))
			{
				for (auto i = 0; i < 16; i++)
				{
					auto teleportHeaderText = fmt::format("Teleport Slot: {0:d}", i);
					if (ImGui::CollapsingHeader(teleportHeaderText.c_str()))
					{
						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextX[sizeof"###TeleportDestinationX15"];
						sprintf_s(teleportDestinationTextX, "###TeleportDestinationX%d", i);
						ImGui::InputFloat(teleportDestinationTextX, &ErectusIni::customTeleportSettings.teleportEntryData[i].destination[0]);
						ImGui::SameLine(122.0f);

						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextY[sizeof"###TeleportDestinationY15"];
						sprintf_s(teleportDestinationTextY, "###TeleportDestinationY%d", i);
						ImGui::InputFloat(teleportDestinationTextY, &ErectusIni::customTeleportSettings.teleportEntryData[i].destination[1]);
						ImGui::SameLine(235.0f);

						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextZ[sizeof"###TeleportDestinationZ15"];
						sprintf_s(teleportDestinationTextZ, "###TeleportDestinationZ%d", i);
						ImGui::InputFloat(teleportDestinationTextZ, &ErectusIni::customTeleportSettings.teleportEntryData[i].destination[2]);
						ImGui::SameLine(349.0f);

						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextW[sizeof"###TeleportDestinationW15"];
						sprintf_s(teleportDestinationTextW, "###TeleportDestinationW%d", i);
						ImGui::InputFloat(teleportDestinationTextW, &ErectusIni::customTeleportSettings.teleportEntryData[i].destination[3]);

						char formIdLabelText[sizeof"###TeleportCellFormId15"];
						sprintf_s(formIdLabelText, "###TeleportCellFormId%d", i);
						char formIdText[sizeof"00000000"];
						sprintf_s(formIdText, "%08lX", ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId);
						ImGui::SetNextItemWidth(110.0f);
						ImGui::InputText(formIdLabelText, formIdText, sizeof formIdText, ImGuiInputTextFlags_CharsHexadecimal);
						sscanf_s(formIdText, "%08lX", &ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId);

						ImGui::SameLine(122.0f);
						char teleportDestinationEnabledText[sizeof"Set Position###TeleportDestinationEnabled15"];
						sprintf_s(teleportDestinationEnabledText, "Set Position###TeleportDestinationEnabled%d", i);
						char teleportDestinationDisabledText[sizeof"Set Position###TeleportDestinationDisabled15"];
						sprintf_s(teleportDestinationDisabledText, "Set Position###TeleportDestinationDisabled%d", i);
						if (!ErectusIni::customTeleportSettings.teleportEntryData[i].disableSaving)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
							if (ImGui::Button(teleportDestinationEnabledText, ImVec2(110.0f, 0.0f)))
								ErectusMemory::GetTeleportPosition(i);
							ImGui::PopStyleColor(3);
						}
						else
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
							ImGui::Button(teleportDestinationDisabledText, ImVec2(110.0f, 0.0f));
							ImGui::PopStyleColor(3);
							ImGui::PopItemFlag();
						}
						ImGui::SameLine(235.0f);

						{
							auto buttonLabel = fmt::format("Lock###DisableSaving{:d}", i);
							SmallButtonToggle(buttonLabel.c_str(), &ErectusIni::customTeleportSettings.teleportEntryData[i].disableSaving);
						}
						
						ImGui::SameLine(349.0f);

						if (ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

							auto buttonLabel = fmt::format("Teleport###TeleportRequestEnabled{:d}", i);
							if (ImGui::Button(buttonLabel.c_str(), ImVec2(110.0f, 0.0f)))
								ErectusMemory::RequestTeleport(i);
							ImGui::PopStyleColor(3);
						}
						else
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

							auto buttonLabel = fmt::format("Teleport###TeleportRequestDisabled{:d}", i);
							ImGui::Button(buttonLabel.c_str(), ImVec2(110.0f, 0.0f));
							ImGui::PopStyleColor(3);
							ImGui::PopItemFlag();
						}
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("BitMsgWriter###BitMsgWriterTab"))
			{
				LargeButtonToggle("Message Sender Enabled", &ErectusMemory::allowMessages);

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

bool ErectusImGui::ImGuiInitialize()
{
	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(ErectusMain::appHwnd))
		return false;

	if (!ImGui_ImplDX9_Init(Renderer::d3D9Device))
		return false;

	return true;
}

void ErectusImGui::ImGuiCleanup()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
