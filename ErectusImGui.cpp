#include "ErectusInclude.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

bool ErectusImGui::DragMenu()
{
	if (!GetCursorPos(&pointerPosition)) return false;

	ErectusMain::windowPosition[0] = pointerPosition.x - static_cast<int>(pointerOrigin.x);
	ErectusMain::windowPosition[1] = pointerPosition.y - static_cast<int>(pointerOrigin.y);

	const auto screenX = GetSystemMetrics(SM_CXSCREEN);
	const auto screenY = GetSystemMetrics(SM_CYSCREEN);

	if (ErectusMain::windowPosition[0] < 0)
	{
		ErectusMain::windowPosition[0] = 0;
	}

	if (ErectusMain::windowPosition[1] < 0)
	{
		ErectusMain::windowPosition[1] = 0;
	}

	if (ErectusMain::windowPosition[0] > (screenX - ErectusMain::windowSize[0]))
	{
		ErectusMain::windowPosition[0] = (screenX - ErectusMain::windowSize[0]);
	}

	if (ErectusMain::windowPosition[1] > (screenY - ErectusMain::windowSize[1]))
	{
		ErectusMain::windowPosition[1] = (screenY - ErectusMain::windowSize[1]);
	}

	return MoveWindow(ErectusMain::windowHwnd, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1],
		ErectusMain::windowSize[0], ErectusMain::windowSize[1], FALSE);
}

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

				ImGui::MenuItem("Overlay Menu");
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

		ImGui::SetNextItemWidth(static_cast<float>(ErectusMain::windowSize[0]) - 16.0f);

		auto processText = ErectusProcess::pid ? "Fallout76.exe - " + std::to_string(ErectusProcess::pid) : "No  process selected.";
		if (ImGui::BeginCombo("###ProcessList", processText.c_str()))
		{
			for(auto item : ErectusProcess::GetProcesses())
			{
				processText = item ? "Fallout76.exe - " + std::to_string(item) : "NONE";
				if (ImGui::Selectable(processText.c_str()))
				{
					ErectusProcess::AttachToProcess(item);
				}
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

		if (ImGui::IsMouseDragging(0) && allowDrag)
		{
			if (!pointerDrag)
			{
				pointerOrigin = ImGui::GetMousePos();
				pointerDrag = true;
			}
		}
		else
		{
			if (pointerDrag)
			{
				pointerOrigin = { 0.0f, 0.0f };
				pointerDrag = false;
			}
		}

		if (pointerDrag)
		{
			DragMenu();
		}
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
		if (ImGui::Button(label, ImVec2(224.0f, 0.0f))) *state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(224.0f, 0.0f))) *state = true;
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
		if (ImGui::Button(label, ImVec2(451.0f, 0.0f))) *state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(451.0f, 0.0f))) *state = true;
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
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f))) *state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f))) *state = true;
		ImGui::PopStyleColor(3);
	}
}

void ErectusImGui::OverlayMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ErectusMain::windowSize[0]),
		static_cast<float>(ErectusMain::windowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	auto allowDrag = true;
	if (ImGui::Begin("Erectus - Overlay Menu", nullptr,
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
			{
				ErectusMain::CloseWnd();
			}

			if (ImGui::MenuItem("Process Menu"))
			{
				ErectusProcess::SetProcessMenu();
			}

			if (ImGui::MenuItem("Overlay"))
			{
				if (!ErectusMain::SetOverlayPosition(false, true))
				{
					ErectusProcess::SetProcessMenu();
				}
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
					ImGui::SliderInt("###PlayerSettingsEnabledDistance", &ErectusIni::playerSettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::playerSettings.enabledDistance, 0, 3000);

					ButtonToggle("Draw Living Players", &ErectusIni::playerSettings.drawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsAliveColor", ErectusIni::playerSettings.aliveColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::playerSettings.aliveColor);

					ButtonToggle("Draw Downed Players", &ErectusIni::playerSettings.drawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDownedColor", ErectusIni::playerSettings.downedColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::playerSettings.downedColor);

					ButtonToggle("Draw Dead Players", &ErectusIni::playerSettings.drawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDeadColor", ErectusIni::playerSettings.deadColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::playerSettings.deadColor);

					ButtonToggle("Draw Unknown Players", &ErectusIni::playerSettings.drawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsUnknownColor", ErectusIni::playerSettings.unknownColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::playerSettings.unknownColor);

					ButtonToggle("Draw Enabled Players", &ErectusIni::playerSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsEnabledAlpha", &ErectusIni::playerSettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::playerSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Players", &ErectusIni::playerSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsDisabledAlpha", &ErectusIni::playerSettings.disabledAlpha,
						0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::playerSettings.disabledAlpha, 0.0f, 1.0f);

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
					ImGui::SliderInt("###NpcSettingsEnabledDistance", &ErectusIni::npcSettings.enabledDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::npcSettings.enabledDistance, 0, 3000);

					ButtonToggle("Draw Living NPCs", &ErectusIni::npcSettings.drawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsAliveColor", ErectusIni::npcSettings.aliveColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::npcSettings.aliveColor);

					ButtonToggle("Draw Downed NPCs", &ErectusIni::npcSettings.drawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDownedColor", ErectusIni::npcSettings.downedColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::npcSettings.downedColor);

					ButtonToggle("Draw Dead NPCs", &ErectusIni::npcSettings.drawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDeadColor", ErectusIni::npcSettings.deadColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::npcSettings.deadColor);

					ButtonToggle("Draw Unknown NPCs", &ErectusIni::npcSettings.drawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsUnknownColor", ErectusIni::npcSettings.unknownColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::npcSettings.unknownColor);

					ButtonToggle("Draw Enabled NPCs", &ErectusIni::npcSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsEnabledAlpha", &ErectusIni::npcSettings.enabledAlpha, 0.0f, 1.0f,
						"Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::npcSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled NPCs", &ErectusIni::npcSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsDisabledAlpha", &ErectusIni::npcSettings.disabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::npcSettings.disabledAlpha, 0.0f, 1.0f);

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

					ButtonToggle("Always Draw Living 1* NPCs",
						&ErectusIni::customLegendarySettings.overrideLivingOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingOneStarColor", ErectusIni::customLegendarySettings.livingOneStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingOneStarColor);

					ButtonToggle("Always Draw Dead 1* NPCs", &ErectusIni::customLegendarySettings.overrideDeadOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadOneStarColor", ErectusIni::customLegendarySettings.deadOneStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadOneStarColor);

					ButtonToggle("Always Draw Living 2* NPCs",
						&ErectusIni::customLegendarySettings.overrideLivingTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingTwoStarColor", ErectusIni::customLegendarySettings.livingTwoStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingTwoStarColor);

					ButtonToggle("Always Draw Dead 2* NPCs", &ErectusIni::customLegendarySettings.overrideDeadTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadTwoStarColor", ErectusIni::customLegendarySettings.deadTwoStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadTwoStarColor);

					ButtonToggle("Always Draw Living 3* NPCs",
						&ErectusIni::customLegendarySettings.overrideLivingThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingThreeStarColor",
						ErectusIni::customLegendarySettings.livingThreeStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.livingThreeStarColor);

					ButtonToggle("Always Draw Dead 3* NPCs",
						&ErectusIni::customLegendarySettings.overrideDeadThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadThreeStarColor", ErectusIni::customLegendarySettings.deadThreeStarColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::customLegendarySettings.deadThreeStarColor);

					LargeButtonToggle("Hide NPCs in the Settler Faction",
						&ErectusIni::customExtraNpcSettings.hideSettlerFaction);
					LargeButtonToggle("Hide NPCs in the Crater Raider Faction",
						&ErectusIni::customExtraNpcSettings.hideCraterRaiderFaction);
					LargeButtonToggle("Hide NPCs in the Diehards Faction",
						&ErectusIni::customExtraNpcSettings.hideDieHardFaction);
					LargeButtonToggle("Hide NPCs in the Secret Service Faction",
						&ErectusIni::customExtraNpcSettings.hideSecretServiceFaction);

					LargeButtonToggle("NPC Blacklist Enabled", &ErectusIni::customExtraNpcSettings.useNpcBlacklist);
					if (ImGui::CollapsingHeader("NPC Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							char npcBlacklistEnabledText[sizeof("NPC Blacklist: 63")];
							char npcBlacklistLabelText[sizeof("###NPCBlacklist63")];
							sprintf_s(npcBlacklistEnabledText, "NPC Blacklist: %d", i);
							sprintf_s(npcBlacklistLabelText, "###NPCBlacklist%d", i);
							ButtonToggle(npcBlacklistEnabledText,
								&ErectusIni::customExtraNpcSettings.npcBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::customExtraNpcSettings.npcBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(npcBlacklistLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::customExtraNpcSettings.npcBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container ESP Settings"))
				{
					ButtonToggle("Container ESP Enabled", &ErectusIni::containerSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ContainerSettingsEnabledDistance",
						&ErectusIni::containerSettings.enabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ContainerSettingsColor", ErectusIni::containerSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::containerSettings.color);

					ButtonToggle("Draw Enabled Containers", &ErectusIni::containerSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsEnabledAlpha", &ErectusIni::containerSettings.enabledAlpha,
						0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;

					ButtonToggle("Draw Disabled Containers", &ErectusIni::containerSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsDisabledAlpha",
						&ErectusIni::containerSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;

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
							char whitelistedText[sizeof("Container Whitelist Slot: 31")];
							char whitelistText[sizeof("###ContainerWhitelist31")];
							sprintf_s(whitelistedText, "Container Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###ContainerWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::containerSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::containerSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::containerSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Junk ESP Settings"))
				{
					ButtonToggle("Junk ESP Enabled", &ErectusIni::junkSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###JunkSettingsEnabledDistance", &ErectusIni::junkSettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::junkSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###JunkSettingsColor", ErectusIni::junkSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::junkSettings.color);

					ButtonToggle("Draw Enabled Junk", &ErectusIni::junkSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsEnabledAlpha", &ErectusIni::junkSettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::junkSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Junk", &ErectusIni::junkSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsDisabledAlpha", &ErectusIni::junkSettings.disabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::junkSettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Junk Whitelist Slot: 31")];
							char whitelistText[sizeof("###JunkWhitelist31")];
							sprintf_s(whitelistedText, "Junk Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###JunkWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::junkSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::junkSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::junkSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Plan ESP Settings"))
				{
					ButtonToggle("Plan ESP Enabled", &ErectusIni::planSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###PlanSettingsEnabledDistance", &ErectusIni::planSettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::planSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###PlanSettingsColor", ErectusIni::planSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::planSettings.color);

					ButtonToggle("Draw Enabled Plans", &ErectusIni::planSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsEnabledAlpha", &ErectusIni::planSettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::planSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Plans", &ErectusIni::planSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsDisabledAlpha", &ErectusIni::planSettings.disabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::planSettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Plan Whitelist Slot: 31")];
							char whitelistText[sizeof("###PlanWhitelist31")];
							sprintf_s(whitelistedText, "Plan Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###PlanWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::planSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::planSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::planSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Magazine ESP Settings"))
				{
					ButtonToggle("Magazine ESP Enabled", &ErectusIni::magazineSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###MagazineSettingsEnabledDistance",
						&ErectusIni::magazineSettings.enabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::magazineSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###MagazineSettingsColor", ErectusIni::magazineSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::magazineSettings.color);

					ButtonToggle("Draw Enabled Magazines", &ErectusIni::magazineSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsEnabledAlpha", &ErectusIni::magazineSettings.enabledAlpha,
						0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::magazineSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Magazines", &ErectusIni::magazineSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsDisabledAlpha", &ErectusIni::magazineSettings.disabledAlpha,
						0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::magazineSettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Magazine Whitelist Slot: 31")];
							char whitelistText[sizeof("###MagazineWhitelist31")];
							sprintf_s(whitelistedText, "Magazine Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###MagazineWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::magazineSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::magazineSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::magazineSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Bobblehead ESP Settings"))
				{
					ButtonToggle("Bobblehead ESP Enabled", &ErectusIni::bobbleheadSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###BobbleheadSettingsEnabledDistance",
						&ErectusIni::bobbleheadSettings.enabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::bobbleheadSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###BobbleheadSettingsColor", ErectusIni::bobbleheadSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::bobbleheadSettings.color);

					ButtonToggle("Draw Enabled Bobbleheads", &ErectusIni::bobbleheadSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsEnabledAlpha",
						&ErectusIni::bobbleheadSettings.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::bobbleheadSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Bobbleheads", &ErectusIni::bobbleheadSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsDisabledAlpha",
						&ErectusIni::bobbleheadSettings.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::bobbleheadSettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Bobblehead Whitelist Slot: 31")];
							char whitelistText[sizeof("###BobbleheadWhitelist31")];
							sprintf_s(whitelistedText, "Bobblehead Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###BobbleheadWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::bobbleheadSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::bobbleheadSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::bobbleheadSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Item ESP Settings"))
				{
					ButtonToggle("Item ESP Enabled", &ErectusIni::itemSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemSettingsEnabledDistance", &ErectusIni::itemSettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::itemSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ItemSettingsColor", ErectusIni::itemSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::itemSettings.color);

					ButtonToggle("Draw Enabled Items", &ErectusIni::itemSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsEnabledAlpha", &ErectusIni::itemSettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::itemSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Items", &ErectusIni::itemSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsDisabledAlpha", &ErectusIni::itemSettings.disabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::itemSettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Item Whitelist Slot: 31")];
							char whitelistText[sizeof("###ItemWhitelist31")];
							sprintf_s(whitelistedText, "Item Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###ItemWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::itemSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::itemSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::itemSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora ESP Settings"))
				{
					ButtonToggle("Flora ESP Enabled", &ErectusIni::floraSettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###FloraSettingsEnabledDistance", &ErectusIni::floraSettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::floraSettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###FloraSettingsColor", ErectusIni::floraSettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::floraSettings.color);

					ButtonToggle("Draw Enabled Flora", &ErectusIni::floraSettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsEnabledAlpha", &ErectusIni::floraSettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::floraSettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Flora", &ErectusIni::floraSettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsDisabledAlpha", &ErectusIni::floraSettings.disabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::floraSettings.disabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Flora", &ErectusIni::floraSettings.drawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Flora", &ErectusIni::floraSettings.drawUnnamed);

					LargeButtonToggle("Draw Raw Crimson Flux Yielding Flora",
						&ErectusIni::customFluxSettings.crimsonFluxEnabled);
					LargeButtonToggle("Draw Raw Cobalt Flux Yielding Flora",
						&ErectusIni::customFluxSettings.cobaltFluxEnabled);
					LargeButtonToggle("Draw Raw Yellowcake Flux Yielding Flora",
						&ErectusIni::customFluxSettings.yellowcakeFluxEnabled);
					LargeButtonToggle("Draw Raw Fluorescent Flux Yielding Flora",
						&ErectusIni::customFluxSettings.fluorescentFluxEnabled);
					LargeButtonToggle("Draw Raw Violet Flux Yielding Flora",
						&ErectusIni::customFluxSettings.violetFluxEnabled);

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
							char whitelistedText[sizeof("Flora Whitelist Slot: 31")];
							char whitelistText[sizeof("###FloraWhitelist31")];
							sprintf_s(whitelistedText, "Flora Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###FloraWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::floraSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::floraSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::floraSettings.whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Entity ESP Settings"))
				{
					ButtonToggle("Entity ESP Enabled", &ErectusIni::entitySettings.enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###EntitySettingsEnabledDistance", &ErectusIni::entitySettings.enabledDistance, 0,
						3000, "Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::entitySettings.enabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###EntitySettingsColor", ErectusIni::entitySettings.color);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::entitySettings.color);

					ButtonToggle("Draw Enabled Entities", &ErectusIni::entitySettings.drawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsEnabledAlpha", &ErectusIni::entitySettings.enabledAlpha, 0.0f,
						1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::entitySettings.enabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Entities", &ErectusIni::entitySettings.drawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsDisabledAlpha", &ErectusIni::entitySettings.disabledAlpha,
						0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::entitySettings.disabledAlpha, 0.0f, 1.0f);

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
							char whitelistedText[sizeof("Entity Whitelist Slot: 31")];
							char whitelistText[sizeof("###EntityWhitelist31")];
							sprintf_s(whitelistedText, "Entity Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###EntityWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::entitySettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::entitySettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::entitySettings.whitelist[i]);
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
						if (ImGui::Button("Loot Scrap (Keybind: CTRL+E)###LootSelectedScrapEnabled",
							ImVec2(224.0f, 0.0f)))
						{
							ErectusMemory::LootScrap();
						}
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
					ButtonToggle("Scrap Looter Keybind Enabled",
						&ErectusIni::customScrapLooterSettings.scrapKeybindEnabled);

					LargeButtonToggle("Scrap Looter ESP Override (Uses Junk ESP Settings)",
						&ErectusIni::customScrapLooterSettings.scrapOverrideEnabled);

					ButtonToggle("Automatic Looting Enabled###ScrapAutomaticLootingEnabled",
						&ErectusIni::customScrapLooterSettings.scrapAutomaticLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ScrapAutomaticStatus",
						&ErectusIni::customScrapLooterSettings.scrapAutomaticStatus);

					ImGui::SetNextItemWidth(224.0f);
					char scrapAutomaticSpeedMinText[sizeof("Speed (Min): 60 (960 ms)")];
					sprintf_s(scrapAutomaticSpeedMinText, "Speed (Min): %d (%d ms)",
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin,
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin * 16);
					ImGui::SliderInt("###ScrapAutomaticSpeedMin",
						&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin, 10, 60,
						scrapAutomaticSpeedMinText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax <
							ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin)
						{
							ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax =
								ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin;
						}
					}
					Utils::ValidateInt(&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin, 10, 60);
					Utils::ValidateInt(&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax, 10, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char scrapAutomaticSpeedMaxText[sizeof("Speed (Max): 60 (960 ms)")];
					sprintf_s(scrapAutomaticSpeedMaxText, "Speed (Max): %d (%d ms)",
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax,
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax * 16);
					ImGui::SliderInt("###ScrapAutomaticSpeedMax",
						&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax, 10, 60,
						scrapAutomaticSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax <
							ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin)
						{
							ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin =
								ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax;
						}
					}
					Utils::ValidateInt(&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin, 10, 60);
					Utils::ValidateInt(&ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax, 10, 60);
					if (ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax <
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin)
					{
						ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax =
							ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin;
					}

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderInt("###ScrapLooterDistance",
						&ErectusIni::customScrapLooterSettings.scrapLooterDistance, 0, 3000,
						"Scrap Looter Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customScrapLooterSettings.scrapLooterDistance, 0, 3000);

					for (auto i = 0; i < 40; i++)
					{
						ButtonToggle(ErectusIni::customScrapLooterSettings.scrapNameList[i],
							&ErectusIni::customScrapLooterSettings.scrapEnabledList[i]);
						ImGui::SameLine(235.0f);
						char labelText[sizeof("###ScrapReadOnly39")];
						sprintf_s(labelText, "###ScrapReadOnly%d", i);
						char formIdText[sizeof("00000000")];
						sprintf_s(formIdText, "%08lX", ErectusIni::customScrapLooterSettings.scrapFormIdList[i]);
						ImGui::SetNextItemWidth(224.0f);
						ImGui::InputText(labelText, formIdText, sizeof(formIdText), ImGuiInputTextFlags_ReadOnly);
						if (ImGui::IsItemActive()) allowDrag = false;
					}
				}

				if (ImGui::CollapsingHeader("Item Looter"))
				{
					if (ErectusMemory::CheckItemLooterSettings())
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Loot Items (Keybind: CTRL+R)###LootSelectedItemsEnabled",
							ImVec2(224.0f, 0.0f)))
						{
							ErectusMemory::LootItems();
						}
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
					ButtonToggle("Item Looter Keybind Enabled",
						&ErectusIni::customItemLooterSettings.itemKeybindEnabled);

					ButtonToggle("Automatic Looting Enabled###ItemAutomaticLootingEnabled",
						&ErectusIni::customItemLooterSettings.itemAutomaticLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ItemAutomaticStatus",
						&ErectusIni::customItemLooterSettings.itemAutomaticStatus);

					ImGui::SetNextItemWidth(224.0f);
					char itemAutomaticSpeedMinText[sizeof("Speed (Min): 60 (960 ms)")];
					sprintf_s(itemAutomaticSpeedMinText, "Speed (Min): %d (%d ms)",
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin,
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMin",
						&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin, 10, 60,
						itemAutomaticSpeedMinText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax <
							ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin)
						{
							ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax =
								ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin;
						}
					}
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin, 10, 60);
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax, 10, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char itemAutomaticSpeedMaxText[sizeof("Speed (Max): 60 (960 ms)")];
					sprintf_s(itemAutomaticSpeedMaxText, "Speed (Max): %d (%d ms)",
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax,
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMax",
						&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax, 10, 60,
						itemAutomaticSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax <
							ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin)
						{
							ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin =
								ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax;
						}
					}
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin, 10, 60);
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax, 10, 60);
					if (ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax <
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin)
					{
						ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax =
							ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin;
					}

					ButtonToggle("Weapons Enabled###ItemLooterWeaponsEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterWeaponsDistance",
						&ErectusIni::customItemLooterSettings.itemLooterWeaponsDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterWeaponsDistance, 0, 3000);

					ButtonToggle("Armor Enabled###ItemLooterArmorEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterArmorEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterArmorDistance",
						&ErectusIni::customItemLooterSettings.itemLooterArmorDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterArmorDistance, 0, 3000);

					ButtonToggle("Ammo Enabled###ItemLooterAmmoEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAmmoDistance",
						&ErectusIni::customItemLooterSettings.itemLooterAmmoDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterAmmoDistance, 0, 3000);

					ButtonToggle("Mods Enabled###ItemLooterModsEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterModsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterModsDistance",
						&ErectusIni::customItemLooterSettings.itemLooterModsDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterModsDistance, 0, 3000);

					ButtonToggle("Magazines Enabled###ItemLooterMagazinesEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterMagazinesEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMagazinesDistance",
						&ErectusIni::customItemLooterSettings.itemLooterMagazinesDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterMagazinesDistance, 0, 3000);

					ButtonToggle("Bobbleheads Enabled###ItemLooterBobbleheadsEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterBobbleheadsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterBobbleheadsDistance",
						&ErectusIni::customItemLooterSettings.itemLooterBobbleheadsDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterBobbleheadsDistance, 0, 3000);

					ButtonToggle("Aid Enabled###ItemLooterAidEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAidDistance",
						&ErectusIni::customItemLooterSettings.itemLooterAidDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterAidDistance, 0, 3000);

					ButtonToggle("Known Plans Enabled###ItemLooterKnownPlansEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterKnownPlansDistance",
						&ErectusIni::customItemLooterSettings.itemLooterKnownPlansDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterKnownPlansDistance, 0, 3000);

					ButtonToggle("Unknown Plans Enabled###ItemLooterUnknownPlansEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterUnknownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnknownPlansDistance",
						&ErectusIni::customItemLooterSettings.itemLooterUnknownPlansDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterUnknownPlansDistance, 0, 3000);

					ButtonToggle("Misc Enabled###ItemLooterMiscEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMiscDistance",
						&ErectusIni::customItemLooterSettings.itemLooterMiscDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterMiscDistance, 0, 3000);

					ButtonToggle("Other Enabled###ItemLooterUnlistedEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterUnlistedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnlistedDistance",
						&ErectusIni::customItemLooterSettings.itemLooterUnlistedDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterUnlistedDistance, 0, 3000);

					ButtonToggle("Item FormId List Enabled###ItemLooterListEnabled",
						&ErectusIni::customItemLooterSettings.itemLooterListEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterListDistance",
						&ErectusIni::customItemLooterSettings.itemLooterListDistance, 0, 3000,
						"Distance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customItemLooterSettings.itemLooterListDistance, 0, 3000);

					LargeButtonToggle("Item Looter Blacklist Enabled###ItemLooterBlacklistToggle",
						&ErectusIni::customItemLooterSettings.itemLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("Item Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							char itemLooterEnabledText[sizeof("Item Looter Slot: 99")];
							char itemLooterLabelText[sizeof("###ItemLooterList99")];
							sprintf_s(itemLooterEnabledText, "Item Looter Slot: %d", i);
							sprintf_s(itemLooterLabelText, "###ItemLooterList%d", i);
							ButtonToggle(itemLooterEnabledText,
								&ErectusIni::customItemLooterSettings.itemLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX",
								ErectusIni::customItemLooterSettings.itemLooterFormIdList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(itemLooterLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX",
								&ErectusIni::customItemLooterSettings.itemLooterFormIdList[i]);
						}
					}

					if (ImGui::CollapsingHeader("Item Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							char itemLooterBlacklistEnabledText[sizeof("Item Looter Blacklist: 63")];
							char itemLooterBlacklistLabelText[sizeof("###ItemLooterBlacklist63")];
							sprintf_s(itemLooterBlacklistEnabledText, "Item Looter Blacklist: %d", i);
							sprintf_s(itemLooterBlacklistLabelText, "###ItemLooterBlacklist%d", i);
							ButtonToggle(itemLooterBlacklistEnabledText,
								&ErectusIni::customItemLooterSettings.itemLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::customItemLooterSettings.itemLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(itemLooterBlacklistLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::customItemLooterSettings.itemLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("NPC Looter (76m Distance Limit)"))
				{
					LargeButtonToggle("Automatic NPC Looting Enabled (Keybind: CTRL+COMMA)###NPCLooterEnabled",
						&ErectusIni::npcLooterSettings.entityLooterEnabled);

					LargeButtonToggle("Draw NPC Looter Status###NPCLooterStatusEnabled",
						&ErectusIni::npcLooterSettings.entityLooterStatusEnabled);

					ButtonToggle("All Weapons Enabled###NPCLooterAllWeaponsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterAllWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###NPCLooterAllArmorEnabled",
						&ErectusIni::npcLooterSettings.entityLooterAllArmorEnabled);

					ButtonToggle("1* Weapons Enabled###NPCLooterOneStarWeaponsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterOneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###NPCLooterOneStarArmorEnabled",
						&ErectusIni::npcLooterSettings.entityLooterOneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###NPCLooterTwoStarWeaponsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterTwoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###NPCLooterTwoStarArmorEnabled",
						&ErectusIni::npcLooterSettings.entityLooterTwoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###NPCLooterThreeStarWeaponsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterThreeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###NPCLooterThreeStarArmorEnabled",
						&ErectusIni::npcLooterSettings.entityLooterThreeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###NPCLooterAmmoEnabled",
						&ErectusIni::npcLooterSettings.entityLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###NPCLooterModsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterModsEnabled);

					ButtonToggle("Caps Enabled###NPCLooterCapsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterCapsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###NPCLooterJunkEnabled",
						&ErectusIni::npcLooterSettings.entityLooterJunkEnabled);

					ButtonToggle("Aid Enabled###NPCLooterAidEnabled",
						&ErectusIni::npcLooterSettings.entityLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###NPCLooterTreasureMapsEnabled",
						&ErectusIni::npcLooterSettings.entityLooterTreasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###NPCLooterKnownPlansEnabled",
						&ErectusIni::npcLooterSettings.entityLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###NPCLooterUnknownPlansEnabled",
						&ErectusIni::npcLooterSettings.entityLooterUnknownPlansEnabled);

					ButtonToggle("Misc Enabled###NPCLooterMiscEnabled",
						&ErectusIni::npcLooterSettings.entityLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###NPCLooterUnlistedEnabled",
						&ErectusIni::npcLooterSettings.entityLooterUnlistedEnabled);

					LargeButtonToggle("NPC Looter FormId List Enabled###NPCLooterListEnabled",
						&ErectusIni::npcLooterSettings.entityLooterListEnabled);

					LargeButtonToggle("NPC Looter Blacklist Enabled###NPCLooterBlacklistToggle",
						&ErectusIni::npcLooterSettings.entityLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("NPC Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							char npcLooterEnabledText[sizeof("NPC Looter Slot: 99")];
							char npcLooterLabelText[sizeof("###NPCLooterList99")];
							sprintf_s(npcLooterEnabledText, "NPC Looter Slot: %d", i);
							sprintf_s(npcLooterLabelText, "###NPCLooterList%d", i);
							ButtonToggle(npcLooterEnabledText,
								&ErectusIni::npcLooterSettings.entityLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::npcLooterSettings.entityLooterFormIdList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(npcLooterLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::npcLooterSettings.entityLooterFormIdList[i]);
						}
					}

					if (ImGui::CollapsingHeader("NPC Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							char npcLooterBlacklistEnabledText[sizeof("NPC Looter Blacklist: 63")];
							char npcLooterBlacklistLabelText[sizeof("###NPCLooterBlacklist63")];
							sprintf_s(npcLooterBlacklistEnabledText, "NPC Looter Blacklist: %d", i);
							sprintf_s(npcLooterBlacklistLabelText, "###NPCLooterBlacklist%d", i);
							ButtonToggle(npcLooterBlacklistEnabledText,
								&ErectusIni::npcLooterSettings.entityLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::npcLooterSettings.entityLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(npcLooterBlacklistLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::npcLooterSettings.entityLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container Looter (6m Distance Limit)"))
				{
					LargeButtonToggle(
						"Automatic Container Looting Enabled (Keybind: CTRL+PERIOD)###ContainerLooterEnabled",
						&ErectusIni::containerLooterSettings.entityLooterEnabled);

					LargeButtonToggle("Draw Container Looter Status###ContainerLooterStatusEnabled",
						&ErectusIni::containerLooterSettings.entityLooterStatusEnabled);

					ButtonToggle("All Weapons Enabled###ContainerLooterAllWeaponsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterAllWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###ContainerLooterAllArmorEnabled",
						&ErectusIni::containerLooterSettings.entityLooterAllArmorEnabled);

					ButtonToggle("1* Weapons Enabled###ContainerLooterOneStarWeaponsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterOneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###ContainerLooterOneStarArmorEnabled",
						&ErectusIni::containerLooterSettings.entityLooterOneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###ContainerLooterTwoStarWeaponsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterTwoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###ContainerLooterTwoStarArmorEnabled",
						&ErectusIni::containerLooterSettings.entityLooterTwoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###ContainerLooterThreeStarWeaponsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterThreeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###ContainerLooterThreeStarArmorEnabled",
						&ErectusIni::containerLooterSettings.entityLooterThreeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###ContainerLooterAmmoEnabled",
						&ErectusIni::containerLooterSettings.entityLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###ContainerLooterModsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterModsEnabled);

					ButtonToggle("Caps Enabled###ContainerLooterCapsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterCapsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###ContainerLooterJunkEnabled",
						&ErectusIni::containerLooterSettings.entityLooterJunkEnabled);

					ButtonToggle("Aid Enabled###ContainerLooterAidEnabled",
						&ErectusIni::containerLooterSettings.entityLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###ContainerLooterTreasureMapsEnabled",
						&ErectusIni::containerLooterSettings.entityLooterTreasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###ContainerLooterKnownPlansEnabled",
						&ErectusIni::containerLooterSettings.entityLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###ContainerLooterUnknownPlansEnabled",
						&ErectusIni::containerLooterSettings.entityLooterUnknownPlansEnabled);

					ButtonToggle("Misc Enabled###ContainerLooterMiscEnabled",
						&ErectusIni::containerLooterSettings.entityLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###ContainerLooterUnlistedEnabled",
						&ErectusIni::containerLooterSettings.entityLooterUnlistedEnabled);

					LargeButtonToggle("Container Looter FormId List Enabled###ContainerLooterListEnabled",
						&ErectusIni::containerLooterSettings.entityLooterListEnabled);

					LargeButtonToggle("Container Looter Blacklist Enabled###ContainerLooterBlacklistToggle",
						&ErectusIni::containerLooterSettings.entityLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("Container Looter FormId List"))
					{
						for (auto i = 0; i < 100; i++)
						{
							char containerLooterEnabledText[sizeof("Container Looter Slot: 99")];
							char containerLooterLabelText[sizeof("###ContainerLooterList99")];
							sprintf_s(containerLooterEnabledText, "Container Looter Slot: %d", i);
							sprintf_s(containerLooterLabelText, "###ContainerLooterList%d", i);
							ButtonToggle(containerLooterEnabledText,
								&ErectusIni::containerLooterSettings.entityLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX",
								ErectusIni::containerLooterSettings.entityLooterFormIdList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(containerLooterLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX",
								&ErectusIni::containerLooterSettings.entityLooterFormIdList[i]);
						}
					}

					if (ImGui::CollapsingHeader("Container Looter Blacklist"))
					{
						for (auto i = 0; i < 64; i++)
						{
							char containerLooterBlacklistEnabledText[sizeof("Container Looter Blacklist: 63")];
							char containerLooterBlacklistLabelText[sizeof("###ContainerLooterBlacklist63")];
							sprintf_s(containerLooterBlacklistEnabledText, "Container Looter Blacklist: %d", i);
							sprintf_s(containerLooterBlacklistLabelText, "###ContainerLooterBlacklist%d", i);
							ButtonToggle(containerLooterBlacklistEnabledText,
								&ErectusIni::containerLooterSettings.entityLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX",
								ErectusIni::containerLooterSettings.entityLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(containerLooterBlacklistLabelText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX",
								&ErectusIni::containerLooterSettings.entityLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora Harvester (6m Distance Limit)"))
				{
					LargeButtonToggle("Automatic Flora Harvesting Enabled (Keybind: CTRL+P])###HarvesterEnabled",
						&ErectusIni::customHarvesterSettings.harvesterEnabled);

					LargeButtonToggle("Draw Flora Harvester Status###HarvesterStatusEnabled",
						&ErectusIni::customHarvesterSettings.harvesterStatusEnabled);

					LargeButtonToggle("Flora Harvester ESP Override (Uses Flora ESP Settings)",
						&ErectusIni::customHarvesterSettings.harvesterOverrideEnabled);

					for (auto i = 0; i < 69; i++)
					{
						ButtonToggle(ErectusIni::customHarvesterSettings.harvesterNameList[i],
							&ErectusIni::customHarvesterSettings.harvesterEnabledList[i]);
						ImGui::SameLine(235.0f);
						char labelText[sizeof("###HarvesterReadOnly68")];
						sprintf_s(labelText, "###HarvesterReadOnly%d", i);
						char formIdText[sizeof("00000000")];
						sprintf_s(formIdText, "%08lX", ErectusIni::customHarvesterSettings.harvesterFormIdList[i]);
						ImGui::SetNextItemWidth(224.0f);
						ImGui::InputText(labelText, formIdText, sizeof(formIdText), ImGuiInputTextFlags_ReadOnly);
						if (ImGui::IsItemActive()) allowDrag = false;
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
					ImGui::SliderInt("###WeaponCapacity", &ErectusIni::customWeaponSettings.capacity, 0, 999,
						"Capacity: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customWeaponSettings.capacity, 0, 999);

					ButtonToggle("Speed###WeaponSpeedEnabled", &ErectusIni::customWeaponSettings.speedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponSpeed", &ErectusIni::customWeaponSettings.speed, 0.0f, 100.0f,
						"Speed: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customWeaponSettings.speed, 0.0f, 100.0f);

					ButtonToggle("Reach###WeaponReachEnabled", &ErectusIni::customWeaponSettings.reachEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponReach", &ErectusIni::customWeaponSettings.reach, 0.0f, 999.0f,
						"Reach: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customWeaponSettings.reach, 0.0f, 999.0f);
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

					ButtonToggle("Ignore Render Distance###IgnoreRenderDistance",
						&ErectusIni::customTargetSettings.ignoreRenderDistance);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###TargetLockingFOV", &ErectusIni::customTargetSettings.lockingFov, 5.0f, 40.0f,
						"Targeting FOV: %.2f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customTargetSettings.lockingFov, 5.0f, 40.0f);

					ButtonToggle("Ignore Essential NPCs###IgnoreEssentialNPCs",
						&ErectusIni::customTargetSettings.ignoreEssentialNpCs);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###TargetLockingColor", ErectusIni::customTargetSettings.lockingColor);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateRgb(ErectusIni::playerSettings.unknownColor);

					ButtonToggle("Automatic Retargeting###TargetLockingRetargeting",
						&ErectusIni::customTargetSettings.retargeting);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char targetLockingCooldownText[sizeof("Cooldown: 120 (1920 ms)")];
					sprintf_s(targetLockingCooldownText, "Cooldown: %d (%d ms)",
						ErectusIni::customTargetSettings.cooldown,
						ErectusIni::customTargetSettings.cooldown * 16);
					ImGui::SliderInt("###TargetLockingCooldown", &ErectusIni::customTargetSettings.cooldown, 0, 120,
						targetLockingCooldownText);
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customTargetSettings.cooldown, 0, 120);

					ImGui::SetNextItemWidth(224.0f);
					char sendDamageMinText[sizeof("Send Damage (Min): 60 (960 ms)")];
					sprintf_s(sendDamageMinText, "Send Damage (Min): %d (%d ms)",
						ErectusIni::customTargetSettings.sendDamageMin,
						ErectusIni::customTargetSettings.sendDamageMin * 16);
					ImGui::SliderInt("###SendDamageMin", &ErectusIni::customTargetSettings.sendDamageMin, 1, 60,
						sendDamageMinText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.
							sendDamageMin)
						{
							ErectusIni::customTargetSettings.sendDamageMax = ErectusIni::customTargetSettings.
								sendDamageMin;
						}
					}
					Utils::ValidateInt(&ErectusIni::customTargetSettings.sendDamageMin, 1, 60);
					Utils::ValidateInt(&ErectusIni::customTargetSettings.sendDamageMax, 1, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char sendDamageMaxText[sizeof("Send Damage (Max): 60 (960 ms)")];
					sprintf_s(sendDamageMaxText, "Send Damage (Max): %d (%d ms)",
						ErectusIni::customTargetSettings.sendDamageMax,
						ErectusIni::customTargetSettings.sendDamageMax * 16);
					ImGui::SliderInt("###SendDamageMax", &ErectusIni::customTargetSettings.sendDamageMax, 1, 60,
						sendDamageMaxText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.
							sendDamageMin)
						{
							ErectusIni::customTargetSettings.sendDamageMin = ErectusIni::customTargetSettings.
								sendDamageMax;
						}
					}
					Utils::ValidateInt(&ErectusIni::customTargetSettings.sendDamageMin, 1, 60);
					Utils::ValidateInt(&ErectusIni::customTargetSettings.sendDamageMax, 1, 60);
					if (ErectusIni::customTargetSettings.sendDamageMax < ErectusIni::customTargetSettings.sendDamageMin)
					{
						ErectusIni::customTargetSettings.sendDamageMax = ErectusIni::customTargetSettings.sendDamageMin;
					}

					char* favoritedWeaponsPreview = nullptr;
					if (ErectusIni::customTargetSettings.favoriteIndex < 12)
					{
						favoritedWeaponsPreview = ErectusMemory::GetFavoritedWeaponText(
							BYTE(ErectusIni::customTargetSettings.favoriteIndex));
						if (favoritedWeaponsPreview == nullptr)
						{
							favoritedWeaponsPreview = new char[sizeof("[?] Favorited Item InErectus::Valid")];
							sprintf_s(favoritedWeaponsPreview, sizeof("[?] Favorited Item InErectus::Valid"),
								"[%c] Favorited Item InErectus::Valid",
								ErectusMemory::GetFavoriteSlot(
									BYTE(ErectusIni::customTargetSettings.favoriteIndex)));
						}
					}
					else
					{
						favoritedWeaponsPreview = new char[sizeof("[?] No Weapon Selected")];
						sprintf_s(favoritedWeaponsPreview, sizeof("[?] No Weapon Selected"), "[?] No Weapon Selected");
					}

					ImGui::SetNextItemWidth(451.0f);
					if (ImGui::BeginCombo("###BeginTempCombo", favoritedWeaponsPreview))
					{
						favoritedWeaponsArray = ErectusMemory::GetFavoritedWeapons();
						if (favoritedWeaponsArray == nullptr)
						{
							favoritedWeaponsArray = new char* [13];
							favoritedWeaponsArray[0] = new char[sizeof("[?] No Weapon Selected")];
							sprintf_s(favoritedWeaponsArray[0], sizeof("[?] No Weapon Selected"),
								"[?] No Weapon Selected");
							for (auto i = 1; i < 13; i++)
							{
								favoritedWeaponsArray[i] = new char[sizeof("[?] Favorited Item InErectus::Valid")];
								sprintf_s(favoritedWeaponsArray[i], sizeof("[?] Favorited Item InErectus::Valid"),
									"[%c] Favorited Item InErectus::Valid",
									ErectusMemory::GetFavoriteSlot(BYTE(i - 1)));
							}
						}

						for (auto i = 0; i < 13; i++)
						{
							if (ImGui::Selectable(favoritedWeaponsArray[i]))
							{
								if (i)
								{
									ErectusIni::customTargetSettings.favoriteIndex = i - 1;
								}
								else
								{
									ErectusIni::customTargetSettings.favoriteIndex = 12;
								}
							}
						}

						ImGui::EndCombo();
						allowDrag = false;
					}

					if (favoritedWeaponsPreview != nullptr)
					{
						delete[]favoritedWeaponsPreview;
						favoritedWeaponsPreview = nullptr;
					}

					if (favoritedWeaponsArray != nullptr)
					{
						for (auto i = 0; i < 13; i++)
						{
							if (favoritedWeaponsArray[i] != nullptr)
							{
								delete[]favoritedWeaponsArray[i];
								favoritedWeaponsArray[i] = nullptr;
							}
						}

						delete[]favoritedWeaponsArray;
						favoritedWeaponsArray = nullptr;
					}

					Utils::ValidateInt(&ErectusIni::customTargetSettings.favoriteIndex, 0, 12);
				}

				if (ImGui::CollapsingHeader("Melee Settings"))
				{
					LargeButtonToggle("Melee Enabled (Keybind: U)", &ErectusIni::customMeleeSettings.meleeEnabled);

					ImGui::SetNextItemWidth(224.0f);
					char meleeSpeedMinText[sizeof("Melee Speed (Min): 60 (960 ms)")];
					sprintf_s(meleeSpeedMinText, "Melee Speed (Min): %d (%d ms)",
						ErectusIni::customMeleeSettings.meleeSpeedMin,
						ErectusIni::customMeleeSettings.meleeSpeedMin * 16);
					ImGui::SliderInt("###MeleeSpeedMin", &ErectusIni::customMeleeSettings.meleeSpeedMin, 1, 60,
						meleeSpeedMinText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customMeleeSettings.meleeSpeedMax < ErectusIni::customMeleeSettings.
							meleeSpeedMin)
						{
							ErectusIni::customMeleeSettings.meleeSpeedMax = ErectusIni::customMeleeSettings.
								meleeSpeedMin;
						}
					}
					Utils::ValidateInt(&ErectusIni::customMeleeSettings.meleeSpeedMin, 1, 60);
					Utils::ValidateInt(&ErectusIni::customMeleeSettings.meleeSpeedMax, 1, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char meleeSpeedMaxText[sizeof("Melee Speed (Max): 60 (960 ms)")];
					sprintf_s(meleeSpeedMaxText, "Melee Speed (Max): %d (%d ms)",
						ErectusIni::customMeleeSettings.meleeSpeedMax,
						ErectusIni::customMeleeSettings.meleeSpeedMax * 16);
					ImGui::SliderInt("###MeleeSpeedMax", &ErectusIni::customMeleeSettings.meleeSpeedMax, 1, 60,
						meleeSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						allowDrag = false;
						if (ErectusIni::customMeleeSettings.meleeSpeedMax < ErectusIni::customMeleeSettings.
							meleeSpeedMin)
						{
							ErectusIni::customMeleeSettings.meleeSpeedMin = ErectusIni::customMeleeSettings.
								meleeSpeedMax;
						}
					}
					Utils::ValidateInt(&ErectusIni::customMeleeSettings.meleeSpeedMin, 1, 60);
					Utils::ValidateInt(&ErectusIni::customMeleeSettings.meleeSpeedMax, 1, 60);
					if (ErectusIni::customMeleeSettings.meleeSpeedMax < ErectusIni::customMeleeSettings.meleeSpeedMin)
					{
						ErectusIni::customMeleeSettings.meleeSpeedMax = ErectusIni::customMeleeSettings.meleeSpeedMin;
					}
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
					LargeButtonToggle("Position Spoofing (Keybind CTRL+L)##LocalPlayerPositionSpoofingEnabled",
						&ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled);
					ButtonToggle("Draw Position Status###LocalPlayerDrawPositionSpoofingEnabled",
						&ErectusIni::customLocalPlayerSettings.drawPositionSpoofingEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerPositionSpoofingHeight",
						&ErectusIni::customLocalPlayerSettings.positionSpoofingHeight, -524287, 524287,
						"Spoofed Height: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.positionSpoofingHeight, -524287, 524287);

					ButtonToggle("Noclip (Keybind CTRL+Y)###NoclipEnabled",
						&ErectusIni::customLocalPlayerSettings.noclipEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NoclipSpeed", &ErectusIni::customLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f,
						"Speed: %.5f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f);

					ButtonToggle("Client State", &ErectusIni::customLocalPlayerSettings.clientState);
					ImGui::SameLine(235.0f);
					ButtonToggle("Automatic Client State", &ErectusIni::customLocalPlayerSettings.automaticClientState);

					LargeButtonToggle("Freeze Action Points###LocalPlayerFreezeApEnabled",
						&ErectusIni::customLocalPlayerSettings.freezeApEnabled);

					ButtonToggle("Action Points###LocalPlayerAPEnabled",
						&ErectusIni::customLocalPlayerSettings.actionPointsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerAP", &ErectusIni::customLocalPlayerSettings.actionPoints, 0, 99999,
						"Action Points: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.actionPoints, 0, 99999);

					ButtonToggle("Strength###LocalPlayerStrengthEnabled",
						&ErectusIni::customLocalPlayerSettings.strengthEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerStrength", &ErectusIni::customLocalPlayerSettings.strength, 0,
						99999, "Strength: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.strength, 0, 99999);

					ButtonToggle("Perception###LocalPlayerPerceptionEnabled",
						&ErectusIni::customLocalPlayerSettings.perceptionEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerPerception", &ErectusIni::customLocalPlayerSettings.perception, 0,
						99999, "Perception: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.perception, 0, 99999);

					ButtonToggle("Endurance###LocalPlayerEnduranceEnabled",
						&ErectusIni::customLocalPlayerSettings.enduranceEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerEndurance", &ErectusIni::customLocalPlayerSettings.endurance, 0,
						99999, "Endurance: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.endurance, 0, 99999);

					ButtonToggle("Charisma###LocalPlayerCharismaEnabled",
						&ErectusIni::customLocalPlayerSettings.charismaEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerCharisma", &ErectusIni::customLocalPlayerSettings.charisma, 0,
						99999, "Charisma: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.charisma, 0, 99999);

					ButtonToggle("Intelligence###LocalPlayerIntelligenceEnabled",
						&ErectusIni::customLocalPlayerSettings.intelligenceEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerIntelligence", &ErectusIni::customLocalPlayerSettings.intelligence,
						0, 99999, "Intelligence: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.intelligence, 0, 99999);

					ButtonToggle("Agility###LocalPlayerAgilityEnabled",
						&ErectusIni::customLocalPlayerSettings.agilityEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerAgility", &ErectusIni::customLocalPlayerSettings.agility, 0, 99999,
						"Agility: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.agility, 0, 99999);

					ButtonToggle("Luck###LocalPlayerLuckEnabled", &ErectusIni::customLocalPlayerSettings.luckEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerLuck", &ErectusIni::customLocalPlayerSettings.luck, 0, 99999,
						"Luck: %d");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateInt(&ErectusIni::customLocalPlayerSettings.luck, 0, 99999);
				}

				if (ImGui::CollapsingHeader("Character Settings"))
				{
					LargeButtonToggle("Character Appearance Editing Enabled###ChargenEditingEnabled",
						&ErectusIni::customChargenSettings.chargenEditingEnabled);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenThin", &ErectusIni::customChargenSettings.thin, 0.0f, 1.0f,
						"Character Appearance (Thin): %f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customChargenSettings.thin, 0.0f, 1.0f);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenMuscular", &ErectusIni::customChargenSettings.muscular, 0.0f, 1.0f,
						"Character Appearance (Muscular): %f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customChargenSettings.muscular, 0.0f, 1.0f);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenLarge", &ErectusIni::customChargenSettings.large, 0.0f, 1.0f,
						"Character Appearance (Large): %f");
					if (ImGui::IsItemActive()) allowDrag = false;
					Utils::ValidateFloat(&ErectusIni::customChargenSettings.large, 0.0f, 1.0f);
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
						{
							getPtrResult = ErectusMemory::GetPtr(ErectusIni::customUtilitySettings.ptrFormId);
						}
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
					char ptrFormIdText[sizeof("00000000")];
					sprintf_s(ptrFormIdText, "%08lX", ErectusIni::customUtilitySettings.ptrFormId);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###PtrFormIdText", ptrFormIdText, sizeof(ptrFormIdText),
						ImGuiInputTextFlags_CharsHexadecimal))
					{
						getPtrResult = 0;
					}
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(ptrFormIdText, "%08lX", &ErectusIni::customUtilitySettings.ptrFormId);

					ImGui::SameLine(318.0f);
					char ptrPointerText[sizeof("0000000000000000")];
					sprintf_s(ptrPointerText, "%016llX", getPtrResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###PtrPointerText", ptrPointerText, sizeof(ptrPointerText),
						ImGuiInputTextFlags_ReadOnly);
					if (ImGui::IsItemActive()) allowDrag = false;

					if (ErectusIni::customUtilitySettings.addressFormId)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Get Address###GetAddressEnabled", ImVec2(224.0f, 0.0f)))
						{
							getAddressResult = ErectusMemory::GetAddress(ErectusIni::customUtilitySettings.addressFormId);
						}
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
					char addressFormIdText[sizeof("00000000")];
					sprintf_s(addressFormIdText, "%08lX", ErectusIni::customUtilitySettings.addressFormId);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###AddressFormIdText", addressFormIdText, sizeof(addressFormIdText), ImGuiInputTextFlags_CharsHexadecimal))
					{
						getAddressResult = 0;
					}
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(addressFormIdText, "%08lX", &ErectusIni::customUtilitySettings.addressFormId);

					ImGui::SameLine(318.0f);
					char addressPointerText[sizeof("0000000000000000")];
					sprintf_s(addressPointerText, "%016llX", getAddressResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###AddressPointerText", addressPointerText, sizeof(addressPointerText),
						ImGuiInputTextFlags_ReadOnly);
					if (ImGui::IsItemActive()) allowDrag = false;
				}

				if (ImGui::CollapsingHeader("Reference Editor"))
				{
					ButtonToggle("Source FormId###SwapperSourceFormIdToggle", &swapperSourceToggle);
					ImGui::SameLine(235.0f);
					char sourceFormIdText[sizeof("00000000")];
					sprintf_s(sourceFormIdText, "%08lX", ErectusIni::customSwapperSettings.sourceFormId);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperSourceFormIdText", sourceFormIdText, sizeof(sourceFormIdText),
						ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(sourceFormIdText, "%08lX", &ErectusIni::customSwapperSettings.sourceFormId);

					ButtonToggle("Destination FormId###SwapperDestinationFormIdToggle", &swapperDestinationToggle);
					ImGui::SameLine(235.0f);
					char destinationFormIdText[sizeof("00000000")];
					sprintf_s(destinationFormIdText, "%08lX", ErectusIni::customSwapperSettings.destinationFormId);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperDestinationFormIdText", destinationFormIdText,
						sizeof(destinationFormIdText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(destinationFormIdText, "%08lX", &ErectusIni::customSwapperSettings.destinationFormId);

					if (swapperSourceToggle && ErectusIni::customSwapperSettings.sourceFormId &&
						swapperDestinationToggle && ErectusIni::customSwapperSettings.destinationFormId)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceEnabled",
							ImVec2(451.0f, 0.0f)))
						{
							if (ErectusMemory::ReferenceSwap(&ErectusIni::customSwapperSettings.sourceFormId,
								&ErectusIni::customSwapperSettings.destinationFormId))
							{
								ErectusIni::customSwapperSettings.destinationFormId = ErectusIni::customSwapperSettings.
									sourceFormId;
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
						ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceDisabled",
							ImVec2(451.0f, 0.0f));
						ImGui::PopStyleColor(3);
						ImGui::PopItemFlag();
					}
				}

				if (ImGui::CollapsingHeader("Item Transferring"))
				{
					SmallButtonToggle("Source###TransferSourceFormIdToggle", &transferSourceToggle);

					ImGui::SameLine(122.0f);
					char sourceFormIdText[sizeof("00000000")];
					sprintf_s(sourceFormIdText, "%08lX", ErectusIni::customTransferSettings.sourceFormId);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferSourceFormIdText", sourceFormIdText, sizeof(sourceFormIdText),
						ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(sourceFormIdText, "%08lX", &ErectusIni::customTransferSettings.sourceFormId);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferSourceLocalPlayer", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::customTransferSettings.sourceFormId = ErectusMemory::GetLocalPlayerFormId();
					}
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferSourceSTASH", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::customTransferSettings.sourceFormId = ErectusMemory::GetStashFormId();
					}
					ImGui::PopStyleColor(3);

					SmallButtonToggle("Destination###TransferDestinationFormIdToggle", &transferDestinationToggle);
					ImGui::SameLine(122.0f);
					char destinationFormIdText[sizeof("00000000")];
					sprintf_s(destinationFormIdText, "%08lX", ErectusIni::customTransferSettings.destinationFormId);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferDestinationFormIdText", destinationFormIdText,
						sizeof(destinationFormIdText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) allowDrag = false;
					sscanf_s(destinationFormIdText, "%08lX", &ErectusIni::customTransferSettings.destinationFormId);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferDestinationLocalPlayer", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::customTransferSettings.destinationFormId = ErectusMemory::GetLocalPlayerFormId();
					}
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferDestinationSTASH", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::customTransferSettings.destinationFormId = ErectusMemory::GetStashFormId();
					}
					ImGui::PopStyleColor(3);

					auto allowTransfer = false;

					if (transferSourceToggle && ErectusIni::customTransferSettings.sourceFormId &&
						transferDestinationToggle && ErectusIni::customTransferSettings.destinationFormId)
					{
						if (ErectusIni::customTransferSettings.useWhitelist)
						{
							allowTransfer = ErectusMemory::CheckItemTransferList();
						}
						else
						{
							allowTransfer = true;
						}
					}

					if (allowTransfer)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Transfer Items###TransferItemsEnabled", ImVec2(451.0f, 0.0f)))
						{
							ErectusMemory::TransferItems(ErectusIni::customTransferSettings.sourceFormId,
								ErectusIni::customTransferSettings.destinationFormId);
						}
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
							char whitelistedText[sizeof("Transfer Whitelist Slot: 31")];
							char whitelistText[sizeof("###ItemTransferWhitelist31")];
							sprintf_s(whitelistedText, "Transfer Whitelist Slot: %d", i);
							sprintf_s(whitelistText, "###ItemTransferWhitelist%d", i);
							ButtonToggle(whitelistedText, &ErectusIni::customTransferSettings.whitelisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::customTransferSettings.whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(whitelistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::customTransferSettings.whitelist[i]);
						}
					}

					if (ImGui::CollapsingHeader("Item Transfer Blacklist Settings"))
					{
						for (auto i = 0; i < 32; i++)
						{
							char blacklistedText[sizeof("Transfer Blacklist Slot: 31")];
							char blacklistText[sizeof("###ItemTransferBlacklist31")];
							sprintf_s(blacklistedText, "Transfer Blacklist Slot: %d", i);
							sprintf_s(blacklistText, "###ItemTransferBlacklist%d", i);
							ButtonToggle(blacklistedText, &ErectusIni::customTransferSettings.blacklisted[i]);
							ImGui::SameLine(235.0f);
							char formIdText[sizeof("00000000")];
							sprintf_s(formIdText, "%08lX", ErectusIni::customTransferSettings.blacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(blacklistText, formIdText, sizeof(formIdText),
								ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) allowDrag = false;
							sscanf_s(formIdText, "%08lX", &ErectusIni::customTransferSettings.blacklist[i]);
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
					ImGui::Text("%d %d %d %d %d %d %d %d - Alpha", alphaCode[0], alphaCode[1], alphaCode[2],
						alphaCode[3], alphaCode[4], alphaCode[5], alphaCode[6], alphaCode[7]);

					ButtonToggle("Draw Nuke Code Bravo", &ErectusIni::customNukeCodeSettings.drawCodeBravo);
					ImGui::SameLine(255.0f);
					ImGui::Text("%d %d %d %d %d %d %d %d - Bravo", bravoCode[0], bravoCode[1], bravoCode[2],
						bravoCode[3], bravoCode[4], bravoCode[5], bravoCode[6], bravoCode[7]);

					ButtonToggle("Draw Nuke Code Charlie", &ErectusIni::customNukeCodeSettings.drawCodeCharlie);
					ImGui::SameLine(255.0f);
					ImGui::Text("%d %d %d %d %d %d %d %d - Charlie", charlieCode[0], charlieCode[1], charlieCode[2],
						charlieCode[3], charlieCode[4], charlieCode[5], charlieCode[6], charlieCode[7]);
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Teleporter###TeleporterTab"))
			{
				for (auto i = 0; i < 16; i++)
				{
					char teleportHeaderText[sizeof("Teleporter Slot: 15")];
					sprintf_s(teleportHeaderText, "Teleport Slot: %d", i);
					if (ImGui::CollapsingHeader(teleportHeaderText))
					{
						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextX[sizeof("###TeleportDestinationX15")];
						sprintf_s(teleportDestinationTextX, "###TeleportDestinationX%d", i);
						ImGui::InputFloat(teleportDestinationTextX,
							&ErectusIni::customTeleportSettings.teleportEntryData[i].destination[0]);
						if (ImGui::IsItemActive()) allowDrag = false;
						ImGui::SameLine(122.0f);
						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextY[sizeof("###TeleportDestinationY15")];
						sprintf_s(teleportDestinationTextY, "###TeleportDestinationY%d", i);
						ImGui::InputFloat(teleportDestinationTextY,
							&ErectusIni::customTeleportSettings.teleportEntryData[i].destination[1]);
						if (ImGui::IsItemActive()) allowDrag = false;
						ImGui::SameLine(235.0f);
						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextZ[sizeof("###TeleportDestinationZ15")];
						sprintf_s(teleportDestinationTextZ, "###TeleportDestinationZ%d", i);
						ImGui::InputFloat(teleportDestinationTextZ,
							&ErectusIni::customTeleportSettings.teleportEntryData[i].destination[2]);
						if (ImGui::IsItemActive()) allowDrag = false;
						ImGui::SameLine(349.0f);
						ImGui::SetNextItemWidth(110.0f);
						char teleportDestinationTextW[sizeof("###TeleportDestinationW15")];
						sprintf_s(teleportDestinationTextW, "###TeleportDestinationW%d", i);
						ImGui::InputFloat(teleportDestinationTextW,
							&ErectusIni::customTeleportSettings.teleportEntryData[i].destination[3]);
						if (ImGui::IsItemActive()) allowDrag = false;

						char formIdLabelText[sizeof("###TeleportCellFormId15")];
						sprintf_s(formIdLabelText, "###TeleportCellFormId%d", i);
						char formIdText[sizeof("00000000")];
						sprintf_s(formIdText, "%08lX",
							ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId);
						ImGui::SetNextItemWidth(110.0f);
						ImGui::InputText(formIdLabelText, formIdText, sizeof(formIdText),
							ImGuiInputTextFlags_CharsHexadecimal);
						if (ImGui::IsItemActive()) allowDrag = false;
						sscanf_s(formIdText, "%08lX",
							&ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId);
						ImGui::SameLine(122.0f);
						char teleportDestinationEnabledText[sizeof("Set Position###TeleportDestinationEnabled15")];
						sprintf_s(teleportDestinationEnabledText, "Set Position###TeleportDestinationEnabled%d", i);
						char teleportDestinationDisabledText[sizeof("Set Position###TeleportDestinationDisabled15")];
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
						char disableSavingText[sizeof("Lock###DisableSaving15")];
						sprintf_s(disableSavingText, "Lock###DisableSaving%d", i);
						SmallButtonToggle(disableSavingText,
							&ErectusIni::customTeleportSettings.teleportEntryData[i].disableSaving);
						ImGui::SameLine(349.0f);
						char teleportRequestEnabledText[sizeof("Teleport###TeleportRequestEnabled15")];
						sprintf_s(teleportRequestEnabledText, "Teleport###TeleportRequestEnabled%d", i);
						char teleportRequestDisabledText[sizeof("Teleport###TeleportRequestDisabled15")];
						sprintf_s(teleportRequestDisabledText, "Teleport###TeleportRequestDisabled%d", i);
						if (ErectusIni::customTeleportSettings.teleportEntryData[i].cellFormId)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
							if (ImGui::Button(teleportRequestEnabledText, ImVec2(110.0f, 0.0f)))
								ErectusMemory::RequestTeleport(i);
							ImGui::PopStyleColor(3);
						}
						else
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
							ImGui::Button(teleportRequestDisabledText, ImVec2(110.0f, 0.0f));
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

		if (ImGui::GetActiveID() == ImGui::GetWindowScrollbarID(ImGui::GetCurrentWindow(), ImGuiAxis_Y))
		{
			allowDrag = false;
		}

		if (ImGui::IsMouseDragging(0) && allowDrag)
		{
			if (!pointerDrag)
			{
				pointerOrigin = ImGui::GetMousePos();
				pointerDrag = true;
			}
		}
		else if (pointerDrag)
		{
			pointerOrigin = { 0.0f, 0.0f };
			pointerDrag = false;
		}


		if (pointerDrag)
		{
			DragMenu();
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
	imGuiContextCreated = true;

	if (!ImGui_ImplWin32_Init(ErectusMain::windowHwnd)) return false;
	imGuiWin32Initialized = true;

	if (!ImGui_ImplDX9_Init(Renderer::d3D9Device)) return false;
	imGuiD3D9Initialized = true;

	return true;
}

void ErectusImGui::ImGuiCleanup()
{
	if (imGuiD3D9Initialized)
	{
		ImGui_ImplDX9_Shutdown();
		imGuiD3D9Initialized = false;
	}

	if (imGuiWin32Initialized)
	{
		ImGui_ImplWin32_Shutdown();
		imGuiWin32Initialized = false;
	}

	if (imGuiContextCreated)
	{
		ImGui::DestroyContext();
		imGuiContextCreated = false;
	}
}
