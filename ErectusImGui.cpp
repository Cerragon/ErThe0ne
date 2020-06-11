#include "ErectusInclude.h"

bool ErectusImGui::DragMenu()
{
	if (!GetCursorPos(&PointerPosition)) return false;

	ErectusMain::WindowPosition[0] = PointerPosition.x - int(PointerOrigin.x);
	ErectusMain::WindowPosition[1] = PointerPosition.y - int(PointerOrigin.y);

	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	if (ErectusMain::WindowPosition[0] < 0)
	{
		ErectusMain::WindowPosition[0] = 0;
	}

	if (ErectusMain::WindowPosition[1] < 0)
	{
		ErectusMain::WindowPosition[1] = 0;
	}

	if (ErectusMain::WindowPosition[0] > (ScreenX - ErectusMain::WindowSize[0]))
	{
		ErectusMain::WindowPosition[0] = (ScreenX - ErectusMain::WindowSize[0]);
	}

	if (ErectusMain::WindowPosition[1] > (ScreenY - ErectusMain::WindowSize[1]))
	{
		ErectusMain::WindowPosition[1] = (ScreenY - ErectusMain::WindowSize[1]);
	}

	return MoveWindow(ErectusMain::WindowHwnd, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], FALSE);
}

void ErectusImGui::ProcessMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(float(ErectusMain::WindowSize[0]), float(ErectusMain::WindowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	bool AllowDrag = true;
	if (ImGui::Begin("Erectus - Process Menu", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
			{
				ErectusMain::Close();
			}

			if (ErectusProcess::ProcessSelected)
			{
				if (ImGui::MenuItem("Overlay Menu"))
				{
					ErectusMain::SetOverlayMenu();
				}

				if (ImGui::MenuItem("Overlay"))
				{
					ErectusMain::SetOverlayPosition(false, true);
				}
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::MenuItem("Overlay Menu");
				ImGui::MenuItem("Overlay");
				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
			}

			ImGui::EndMenuBar();
		}

		ImGui::SetNextItemWidth(float(ErectusMain::WindowSize[0]) - 16.0f);
		if (ImGui::BeginCombo("###ProcessList", ErectusProcess::ProcessList[ErectusProcess::ProcessIndex]))
		{
			if (!ErectusProcess::ProcessListUpdated)
			{
				ErectusProcess::ProcessListUpdated = true;
				if (!ErectusProcess::UpdateProcessList())
				{
					ErectusProcess::ResetProcessData(true, 1);
				}
			}

			for (int i = 0; i < ErectusProcess::ProcessListSize; i++)
			{
				if (ImGui::Selectable(ErectusProcess::ProcessList[i]))
				{
					ErectusProcess::ProcessIndex = i;
					if (ErectusProcess::ProcessIndex)
					{
						ErectusProcess::ProcessSelected = ErectusProcess::ProcessValid(ErectusProcess::ProcessIdList[ErectusProcess::ProcessIndex]);
						if (!ErectusProcess::ProcessSelected)
						{
							ErectusProcess::ResetProcessData(false, 1);
						}
					}
				}
			}

			ImGui::EndCombo();
			AllowDrag = false;
		}
		else
		{
			if (ErectusProcess::ProcessListUpdated)
			{
				ErectusProcess::ProcessListUpdated = false;
			}
		}

		ImGui::Separator();
		switch (ErectusProcess::ProcessErrorId)
		{
		case 0:
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ErectusProcess::ProcessError);
			break;
		case 1:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ErectusProcess::ProcessError);
			break;
		case 2:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ErectusProcess::ProcessError);
			break;
		default:
			ImGui::Text(ErectusProcess::ProcessError);
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
		ImGui::Text("%lu", ErectusProcess::Pid);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("HWND (Window)");
		ImGui::NextColumn();
		ImGui::Text("%p", ErectusProcess::Hwnd);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("Base Address");
		ImGui::NextColumn();
		ImGui::Text("%016llX", ErectusProcess::Exe);
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("HANDLE");
		ImGui::NextColumn();
		ImGui::Text("%p", ErectusProcess::Handle);
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopItemFlag();

		if (ImGui::IsMouseDragging(0) && AllowDrag)
		{
			if (!PointerDrag)
			{
				PointerOrigin = ImGui::GetMousePos();
				PointerDrag = true;
			}
		}
		else
		{
			if (PointerDrag)
			{
				PointerOrigin = { 0.0f, 0.0f };
				PointerDrag = false;
			}
		}

		if (PointerDrag)
		{
			DragMenu();
		}
	}

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ButtonToggle(const char *Label, bool *State)
{
	if (*State)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(224.0f, 0.0f))) *State = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(224.0f, 0.0f))) *State = true;
		ImGui::PopStyleColor(3);
	}
}

void LargeButtonToggle(const char *Label, bool *State)
{
	if (*State)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(451.0f, 0.0f))) *State = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(451.0f, 0.0f))) *State = true;
		ImGui::PopStyleColor(3);
	}
}

void SmallButtonToggle(const char *Label, bool *State)
{
	if (*State)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(110.0f, 0.0f))) *State = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(Label, ImVec2(110.0f, 0.0f))) *State = true;
		ImGui::PopStyleColor(3);
	}
}

void ErectusImGui::OverlayMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(float(ErectusMain::WindowSize[0]), float(ErectusMain::WindowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	bool AllowDrag = true;
	if (ImGui::Begin("Erectus - Overlay Menu", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
			{
				ErectusMain::Close();
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
					ButtonToggle("Player ESP Enabled", &ErectusIni::PlayerSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###PlayerSettingsEnabledDistance", &ErectusIni::PlayerSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateInt(&ErectusIni::PlayerSettings.EnabledDistance, 0, 3000);

					ButtonToggle("Draw Living Players", &ErectusIni::PlayerSettings.DrawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsAliveColor", ErectusIni::PlayerSettings.AliveColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateRGB(ErectusIni::PlayerSettings.AliveColor);

					ButtonToggle("Draw Downed Players", &ErectusIni::PlayerSettings.DrawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDownedColor", ErectusIni::PlayerSettings.DownedColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateRGB(ErectusIni::PlayerSettings.DownedColor);

					ButtonToggle("Draw Dead Players", &ErectusIni::PlayerSettings.DrawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsDeadColor", ErectusIni::PlayerSettings.DeadColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateRGB(ErectusIni::PlayerSettings.DeadColor);

					ButtonToggle("Draw Unknown Players", &ErectusIni::PlayerSettings.DrawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###PlayerSettingsUnknownColor", ErectusIni::PlayerSettings.UnknownColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateRGB(ErectusIni::PlayerSettings.UnknownColor);

					ButtonToggle("Draw Enabled Players", &ErectusIni::PlayerSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsEnabledAlpha", &ErectusIni::PlayerSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateFloat(&ErectusIni::PlayerSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Players", &ErectusIni::PlayerSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlayerSettingsDisabledAlpha", &ErectusIni::PlayerSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::Erectus::ValidateFloat(&ErectusIni::PlayerSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Players", &ErectusIni::PlayerSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Players", &ErectusIni::PlayerSettings.DrawUnnamed);

					ButtonToggle("Show Player Name", &ErectusIni::PlayerSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Player Distance", &ErectusIni::PlayerSettings.ShowDistance);

					ButtonToggle("Show Player Health", &ErectusIni::PlayerSettings.ShowHealth);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Dead Player Health", &ErectusIni::PlayerSettings.ShowDeadHealth);

					ButtonToggle("Player Text Shadowed", &ErectusIni::PlayerSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Player Text Centered", &ErectusIni::PlayerSettings.TextCentered);
				}

				if (ImGui::CollapsingHeader("NPC ESP Settings"))
				{
					ButtonToggle("NPC ESP Enabled", &ErectusIni::NpcSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###NpcSettingsEnabledDistance", &ErectusIni::NpcSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::NpcSettings.EnabledDistance, 0, 3000);

					ButtonToggle("Draw Living NPCs", &ErectusIni::NpcSettings.DrawAlive);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsAliveColor", ErectusIni::NpcSettings.AliveColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::NpcSettings.AliveColor);

					ButtonToggle("Draw Downed NPCs", &ErectusIni::NpcSettings.DrawDowned);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDownedColor", ErectusIni::NpcSettings.DownedColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::NpcSettings.DownedColor);

					ButtonToggle("Draw Dead NPCs", &ErectusIni::NpcSettings.DrawDead);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsDeadColor", ErectusIni::NpcSettings.DeadColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::NpcSettings.DeadColor);

					ButtonToggle("Draw Unknown NPCs", &ErectusIni::NpcSettings.DrawUnknown);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###NpcSettingsUnknownColor", ErectusIni::NpcSettings.UnknownColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::NpcSettings.UnknownColor);

					ButtonToggle("Draw Enabled NPCs", &ErectusIni::NpcSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsEnabledAlpha", &ErectusIni::NpcSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::NpcSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled NPCs", &ErectusIni::NpcSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NpcSettingsDisabledAlpha", &ErectusIni::NpcSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::NpcSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named NPCs", &ErectusIni::NpcSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed NPCs", &ErectusIni::NpcSettings.DrawUnnamed);

					ButtonToggle("Show NPC Name", &ErectusIni::NpcSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show NPC Distance", &ErectusIni::NpcSettings.ShowDistance);

					ButtonToggle("Show NPC Health", &ErectusIni::NpcSettings.ShowHealth);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Dead NPC Health", &ErectusIni::NpcSettings.ShowDeadHealth);

					ButtonToggle("NPC Text Shadowed", &ErectusIni::NpcSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("NPC Text Centered", &ErectusIni::NpcSettings.TextCentered);

					ButtonToggle("Always Draw Living 1* NPCs", &ErectusIni::CustomLegendarySettings.OverrideLivingOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingOneStarColor", ErectusIni::CustomLegendarySettings.LivingOneStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.LivingOneStarColor);

					ButtonToggle("Always Draw Dead 1* NPCs", &ErectusIni::CustomLegendarySettings.OverrideDeadOneStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadOneStarColor", ErectusIni::CustomLegendarySettings.DeadOneStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.DeadOneStarColor);

					ButtonToggle("Always Draw Living 2* NPCs", &ErectusIni::CustomLegendarySettings.OverrideLivingTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingTwoStarColor", ErectusIni::CustomLegendarySettings.LivingTwoStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.LivingTwoStarColor);

					ButtonToggle("Always Draw Dead 2* NPCs", &ErectusIni::CustomLegendarySettings.OverrideDeadTwoStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadTwoStarColor", ErectusIni::CustomLegendarySettings.DeadTwoStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.DeadTwoStarColor);

					ButtonToggle("Always Draw Living 3* NPCs", &ErectusIni::CustomLegendarySettings.OverrideLivingThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###LivingThreeStarColor", ErectusIni::CustomLegendarySettings.LivingThreeStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.LivingThreeStarColor);

					ButtonToggle("Always Draw Dead 3* NPCs", &ErectusIni::CustomLegendarySettings.OverrideDeadThreeStar);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###DeadThreeStarColor", ErectusIni::CustomLegendarySettings.DeadThreeStarColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::CustomLegendarySettings.DeadThreeStarColor);
					
					LargeButtonToggle("Hide NPCs in the Settler Faction", &ErectusIni::CustomExtraNPCSettings.HideSettlerFaction);
					LargeButtonToggle("Hide NPCs in the Crater Raider Faction", &ErectusIni::CustomExtraNPCSettings.HideCraterRaiderFaction);
					LargeButtonToggle("Hide NPCs in the Diehards Faction", &ErectusIni::CustomExtraNPCSettings.HideDieHardFaction);
					LargeButtonToggle("Hide NPCs in the Secret Service Faction", &ErectusIni::CustomExtraNPCSettings.HideSecretServiceFaction);

					LargeButtonToggle("NPC Blacklist Enabled", &ErectusIni::CustomExtraNPCSettings.UseNPCBlacklist);
					if (ImGui::CollapsingHeader("NPC Blacklist"))
					{
						for (int i = 0; i < 64; i++)
						{
							char NPCBlacklistEnabledText[sizeof("NPC Blacklist: 63")];
							char NPCBlacklistLabelText[sizeof("###NPCBlacklist63")];
							sprintf_s(NPCBlacklistEnabledText, "NPC Blacklist: %d", i);
							sprintf_s(NPCBlacklistLabelText, "###NPCBlacklist%d", i);
							ButtonToggle(NPCBlacklistEnabledText, &ErectusIni::CustomExtraNPCSettings.NPCBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::CustomExtraNPCSettings.NPCBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(NPCBlacklistLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::CustomExtraNPCSettings.NPCBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container ESP Settings"))
				{
					ButtonToggle("Container ESP Enabled", &ErectusIni::ContainerSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ContainerSettingsEnabledDistance", &ErectusIni::ContainerSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::ContainerSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ContainerSettingsColor", ErectusIni::ContainerSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::ContainerSettings.Color);

					ButtonToggle("Draw Enabled Containers", &ErectusIni::ContainerSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsEnabledAlpha", &ErectusIni::ContainerSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::ContainerSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Containers", &ErectusIni::ContainerSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ContainerSettingsDisabledAlpha", &ErectusIni::ContainerSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::ContainerSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Containers", &ErectusIni::ContainerSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Containers", &ErectusIni::ContainerSettings.DrawUnnamed);

					ButtonToggle("Show Container Name", &ErectusIni::ContainerSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Container Distance", &ErectusIni::ContainerSettings.ShowDistance);

					ButtonToggle("Container Text Shadowed", &ErectusIni::ContainerSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Container Text Centered", &ErectusIni::ContainerSettings.TextCentered);

					if (ImGui::CollapsingHeader("Container Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Container Whitelist Slot: 31")];
							char WhitelistText[sizeof("###ContainerWhitelist31")];
							sprintf_s(WhitelistedText, "Container Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###ContainerWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::ContainerSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::ContainerSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::ContainerSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Junk ESP Settings"))
				{
					ButtonToggle("Junk ESP Enabled", &ErectusIni::JunkSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###JunkSettingsEnabledDistance", &ErectusIni::JunkSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::JunkSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###JunkSettingsColor", ErectusIni::JunkSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::JunkSettings.Color);

					ButtonToggle("Draw Enabled Junk", &ErectusIni::JunkSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsEnabledAlpha", &ErectusIni::JunkSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::JunkSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Junk", &ErectusIni::JunkSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###JunkSettingsDisabledAlpha", &ErectusIni::JunkSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::JunkSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Junk", &ErectusIni::JunkSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Junk", &ErectusIni::JunkSettings.DrawUnnamed);

					ButtonToggle("Show Junk Name", &ErectusIni::JunkSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Junk Distance", &ErectusIni::JunkSettings.ShowDistance);

					ButtonToggle("Junk Text Shadowed", &ErectusIni::JunkSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Text Centered", &ErectusIni::JunkSettings.TextCentered);

					if (ImGui::CollapsingHeader("Junk Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Junk Whitelist Slot: 31")];
							char WhitelistText[sizeof("###JunkWhitelist31")];
							sprintf_s(WhitelistedText, "Junk Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###JunkWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::JunkSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::JunkSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::JunkSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Plan ESP Settings"))
				{
					ButtonToggle("Plan ESP Enabled", &ErectusIni::PlanSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###PlanSettingsEnabledDistance", &ErectusIni::PlanSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::PlanSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###PlanSettingsColor", ErectusIni::PlanSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::PlanSettings.Color);

					ButtonToggle("Draw Enabled Plans", &ErectusIni::PlanSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsEnabledAlpha", &ErectusIni::PlanSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::PlanSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Plans", &ErectusIni::PlanSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###PlanSettingsDisabledAlpha", &ErectusIni::PlanSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::PlanSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Known Plans", &ErectusIni::CustomKnownRecipeSettings.KnownRecipesEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unknown Plans", &ErectusIni::CustomKnownRecipeSettings.UnknownRecipesEnabled);

					ButtonToggle("Draw Named Plans", &ErectusIni::PlanSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Plans", &ErectusIni::PlanSettings.DrawUnnamed);

					ButtonToggle("Show Plan Name", &ErectusIni::PlanSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Plan Distance", &ErectusIni::PlanSettings.ShowDistance);

					ButtonToggle("Plan Text Shadowed", &ErectusIni::PlanSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Plan Text Centered", &ErectusIni::PlanSettings.TextCentered);

					if (ImGui::CollapsingHeader("Plan Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Plan Whitelist Slot: 31")];
							char WhitelistText[sizeof("###PlanWhitelist31")];
							sprintf_s(WhitelistedText, "Plan Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###PlanWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::PlanSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::PlanSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::PlanSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Magazine ESP Settings"))
				{
					ButtonToggle("Magazine ESP Enabled", &ErectusIni::MagazineSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###MagazineSettingsEnabledDistance", &ErectusIni::MagazineSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::MagazineSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###MagazineSettingsColor", ErectusIni::MagazineSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::MagazineSettings.Color);

					ButtonToggle("Draw Enabled Magazines", &ErectusIni::MagazineSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsEnabledAlpha", &ErectusIni::MagazineSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::MagazineSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Magazines", &ErectusIni::MagazineSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###MagazineSettingsDisabledAlpha", &ErectusIni::MagazineSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::MagazineSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Magazines", &ErectusIni::MagazineSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Magazines", &ErectusIni::MagazineSettings.DrawUnnamed);

					ButtonToggle("Show Magazine Name", &ErectusIni::MagazineSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Magazine Distance", &ErectusIni::MagazineSettings.ShowDistance);

					ButtonToggle("Magazine Text Shadowed", &ErectusIni::MagazineSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Magazine Text Centered", &ErectusIni::MagazineSettings.TextCentered);

					if (ImGui::CollapsingHeader("Magazine Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Magazine Whitelist Slot: 31")];
							char WhitelistText[sizeof("###MagazineWhitelist31")];
							sprintf_s(WhitelistedText, "Magazine Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###MagazineWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::MagazineSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::MagazineSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::MagazineSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Bobblehead ESP Settings"))
				{
					ButtonToggle("Bobblehead ESP Enabled", &ErectusIni::BobbleheadSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###BobbleheadSettingsEnabledDistance", &ErectusIni::BobbleheadSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::BobbleheadSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###BobbleheadSettingsColor", ErectusIni::BobbleheadSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::BobbleheadSettings.Color);

					ButtonToggle("Draw Enabled Bobbleheads", &ErectusIni::BobbleheadSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsEnabledAlpha", &ErectusIni::BobbleheadSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::BobbleheadSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Bobbleheads", &ErectusIni::BobbleheadSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###BobbleheadSettingsDisabledAlpha", &ErectusIni::BobbleheadSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::BobbleheadSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Bobbleheads", &ErectusIni::BobbleheadSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Bobbleheads", &ErectusIni::BobbleheadSettings.DrawUnnamed);

					ButtonToggle("Show Bobblehead Name", &ErectusIni::BobbleheadSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Bobblehead Distance", &ErectusIni::BobbleheadSettings.ShowDistance);

					ButtonToggle("Bobblehead Text Shadowed", &ErectusIni::BobbleheadSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Bobblehead Text Centered", &ErectusIni::BobbleheadSettings.TextCentered);

					if (ImGui::CollapsingHeader("Bobblehead Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Bobblehead Whitelist Slot: 31")];
							char WhitelistText[sizeof("###BobbleheadWhitelist31")];
							sprintf_s(WhitelistedText, "Bobblehead Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###BobbleheadWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::BobbleheadSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::BobbleheadSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::BobbleheadSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Item ESP Settings"))
				{
					ButtonToggle("Item ESP Enabled", &ErectusIni::ItemSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemSettingsEnabledDistance", &ErectusIni::ItemSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::ItemSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###ItemSettingsColor", ErectusIni::ItemSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::ItemSettings.Color);

					ButtonToggle("Draw Enabled Items", &ErectusIni::ItemSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsEnabledAlpha", &ErectusIni::ItemSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::ItemSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Items", &ErectusIni::ItemSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###ItemSettingsDisabledAlpha", &ErectusIni::ItemSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::ItemSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Items", &ErectusIni::ItemSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Items", &ErectusIni::ItemSettings.DrawUnnamed);

					ButtonToggle("Show Item Name", &ErectusIni::ItemSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Item Distance", &ErectusIni::ItemSettings.ShowDistance);

					ButtonToggle("Item Text Shadowed", &ErectusIni::ItemSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Item Text Centered", &ErectusIni::ItemSettings.TextCentered);

					if (ImGui::CollapsingHeader("Item Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Item Whitelist Slot: 31")];
							char WhitelistText[sizeof("###ItemWhitelist31")];
							sprintf_s(WhitelistedText, "Item Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###ItemWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::ItemSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::ItemSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::ItemSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora ESP Settings"))
				{
					ButtonToggle("Flora ESP Enabled", &ErectusIni::FloraSettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###FloraSettingsEnabledDistance", &ErectusIni::FloraSettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::FloraSettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###FloraSettingsColor", ErectusIni::FloraSettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::FloraSettings.Color);

					ButtonToggle("Draw Enabled Flora", &ErectusIni::FloraSettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsEnabledAlpha", &ErectusIni::FloraSettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::FloraSettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Flora", &ErectusIni::FloraSettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###FloraSettingsDisabledAlpha", &ErectusIni::FloraSettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::FloraSettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Flora", &ErectusIni::FloraSettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Flora", &ErectusIni::FloraSettings.DrawUnnamed);

					LargeButtonToggle("Draw Raw Crimson Flux Yielding Flora", &ErectusIni::CustomFluxSettings.CrimsonFluxEnabled);
					LargeButtonToggle("Draw Raw Cobalt Flux Yielding Flora", &ErectusIni::CustomFluxSettings.CobaltFluxEnabled);
					LargeButtonToggle("Draw Raw Yellowcake Flux Yielding Flora", &ErectusIni::CustomFluxSettings.YellowcakeFluxEnabled);
					LargeButtonToggle("Draw Raw Fluorescent Flux Yielding Flora", &ErectusIni::CustomFluxSettings.FluorescentFluxEnabled);
					LargeButtonToggle("Draw Raw Violet Flux Yielding Flora", &ErectusIni::CustomFluxSettings.VioletFluxEnabled);

					ButtonToggle("Show Flora Name", &ErectusIni::FloraSettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Flora Distance", &ErectusIni::FloraSettings.ShowDistance);

					ButtonToggle("Flora Text Shadowed", &ErectusIni::FloraSettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Flora Text Centered", &ErectusIni::FloraSettings.TextCentered);

					if (ImGui::CollapsingHeader("Flora Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Flora Whitelist Slot: 31")];
							char WhitelistText[sizeof("###FloraWhitelist31")];
							sprintf_s(WhitelistedText, "Flora Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###FloraWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::FloraSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::FloraSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::FloraSettings.Whitelist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Entity ESP Settings"))
				{
					ButtonToggle("Entity ESP Enabled", &ErectusIni::EntitySettings.Enabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###EntitySettingsEnabledDistance", &ErectusIni::EntitySettings.EnabledDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::EntitySettings.EnabledDistance, 0, 3000);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::ColorEdit3("###EntitySettingsColor", ErectusIni::EntitySettings.Color);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::EntitySettings.Color);

					ButtonToggle("Draw Enabled Entities", &ErectusIni::EntitySettings.DrawEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsEnabledAlpha", &ErectusIni::EntitySettings.EnabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::EntitySettings.EnabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Disabled Entities", &ErectusIni::EntitySettings.DrawDisabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###EntitySettingsDisabledAlpha", &ErectusIni::EntitySettings.DisabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::EntitySettings.DisabledAlpha, 0.0f, 1.0f);

					ButtonToggle("Draw Named Entities", &ErectusIni::EntitySettings.DrawNamed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Unnamed Entities", &ErectusIni::EntitySettings.DrawUnnamed);

					ButtonToggle("Show Entity Name", &ErectusIni::EntitySettings.ShowName);
					ImGui::SameLine(235.0f);
					ButtonToggle("Show Entity Distance", &ErectusIni::EntitySettings.ShowDistance);

					ButtonToggle("Entity Text Shadowed", &ErectusIni::EntitySettings.TextShadowed);
					ImGui::SameLine(235.0f);
					ButtonToggle("Entity Text Centered", &ErectusIni::EntitySettings.TextCentered);

					if (ImGui::CollapsingHeader("Entity Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Entity Whitelist Slot: 31")];
							char WhitelistText[sizeof("###EntityWhitelist31")];
							sprintf_s(WhitelistedText, "Entity Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###EntityWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::EntitySettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::EntitySettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::EntitySettings.Whitelist[i]);
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
					ButtonToggle("Scrap Looter Keybind Enabled", &ErectusIni::CustomScrapLooterSettings.ScrapKeybindEnabled);

					LargeButtonToggle("Scrap Looter ESP Override (Uses Junk ESP Settings)", &ErectusIni::CustomScrapLooterSettings.ScrapOverrideEnabled);

					ButtonToggle("Automatic Looting Enabled###ScrapAutomaticLootingEnabled", &ErectusIni::CustomScrapLooterSettings.ScrapAutomaticLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ScrapAutomaticStatus", &ErectusIni::CustomScrapLooterSettings.ScrapAutomaticStatus);

					ImGui::SetNextItemWidth(224.0f);
					char ScrapAutomaticSpeedMinText[sizeof("Speed (Min): 60 (960 ms)")];
					sprintf_s(ScrapAutomaticSpeedMinText, "Speed (Min): %d (%d ms)", ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin, ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin * 16);
					ImGui::SliderInt("###ScrapAutomaticSpeedMin", &ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin, 10, 60, ScrapAutomaticSpeedMinText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax < ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin)
						{
							ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax = ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin, 10, 60);
					Erectus::ValidateInt(&ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax, 10, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char ScrapAutomaticSpeedMaxText[sizeof("Speed (Max): 60 (960 ms)")];
					sprintf_s(ScrapAutomaticSpeedMaxText, "Speed (Max): %d (%d ms)", ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax, ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax * 16);
					ImGui::SliderInt("###ScrapAutomaticSpeedMax", &ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax, 10, 60, ScrapAutomaticSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax < ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin)
						{
							ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin = ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin, 10, 60);
					Erectus::ValidateInt(&ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax, 10, 60);
					if (ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax < ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin)
					{
						ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax = ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin;
					}

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderInt("###ScrapLooterDistance", &ErectusIni::CustomScrapLooterSettings.ScrapLooterDistance, 0, 3000, "Scrap Looter Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomScrapLooterSettings.ScrapLooterDistance, 0, 3000);

					for (int i = 0; i < 40; i++)
					{
						ButtonToggle(ErectusIni::CustomScrapLooterSettings.ScrapNameList[i], &ErectusIni::CustomScrapLooterSettings.ScrapEnabledList[i]);
						ImGui::SameLine(235.0f);
						char LabelText[sizeof("###ScrapReadOnly39")];
						sprintf_s(LabelText, "###ScrapReadOnly%d", i);
						char FormidText[sizeof("00000000")];
						sprintf_s(FormidText, "%08lX", ErectusIni::CustomScrapLooterSettings.ScrapFormidList[i]);
						ImGui::SetNextItemWidth(224.0f);
						ImGui::InputText(LabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_ReadOnly);
						if (ImGui::IsItemActive()) AllowDrag = false;
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
					ButtonToggle("Item Looter Keybind Enabled", &ErectusIni::CustomItemLooterSettings.ItemKeybindEnabled);

					ButtonToggle("Automatic Looting Enabled###ItemAutomaticLootingEnabled", &ErectusIni::CustomItemLooterSettings.ItemAutomaticLootingEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Draw Automatic Looting Status###ItemAutomaticStatus", &ErectusIni::CustomItemLooterSettings.ItemAutomaticStatus);

					ImGui::SetNextItemWidth(224.0f);
					char ItemAutomaticSpeedMinText[sizeof("Speed (Min): 60 (960 ms)")];
					sprintf_s(ItemAutomaticSpeedMinText, "Speed (Min): %d (%d ms)", ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin, ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMin", &ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin, 10, 60, ItemAutomaticSpeedMinText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax < ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin)
						{
							ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax = ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin, 10, 60);
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax, 10, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char ItemAutomaticSpeedMaxText[sizeof("Speed (Max): 60 (960 ms)")];
					sprintf_s(ItemAutomaticSpeedMaxText, "Speed (Max): %d (%d ms)", ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax, ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax * 16);
					ImGui::SliderInt("###ItemAutomaticSpeedMax", &ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax, 10, 60, ItemAutomaticSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax < ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin)
						{
							ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin = ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin, 10, 60);
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax, 10, 60);
					if (ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax < ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin)
					{
						ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax = ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin;
					}

					ButtonToggle("Weapons Enabled###ItemLooterWeaponsEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterWeaponsDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsDistance, 0, 3000);

					ButtonToggle("Armor Enabled###ItemLooterArmorEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterArmorEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterArmorDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterArmorDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterArmorDistance, 0, 3000);

					ButtonToggle("Ammo Enabled###ItemLooterAmmoEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAmmoDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterAmmoDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterAmmoDistance, 0, 3000);

					ButtonToggle("Mods Enabled###ItemLooterModsEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterModsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterModsDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterModsDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterModsDistance, 0, 3000);

					ButtonToggle("Magazines Enabled###ItemLooterMagazinesEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMagazinesDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesDistance, 0, 3000);

					ButtonToggle("Bobbleheads Enabled###ItemLooterBobbleheadsEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterBobbleheadsDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsDistance, 0, 3000);

					ButtonToggle("Aid Enabled###ItemLooterAidEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterAidDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterAidDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterAidDistance, 0, 3000);

					ButtonToggle("Known Plans Enabled###ItemLooterKnownPlansEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterKnownPlansDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansDistance, 0, 3000);

					ButtonToggle("Unknown Plans Enabled###ItemLooterUnknownPlansEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnknownPlansDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance, 0, 3000);

					ButtonToggle("Misc Enabled###ItemLooterMiscEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterMiscDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterMiscDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterMiscDistance, 0, 3000);

					ButtonToggle("Other Enabled###ItemLooterUnlistedEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterUnlistedDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedDistance, 0, 3000);

					ButtonToggle("Item Formid List Enabled###ItemLooterListEnabled", &ErectusIni::CustomItemLooterSettings.ItemLooterListEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###ItemLooterListDistance", &ErectusIni::CustomItemLooterSettings.ItemLooterListDistance, 0, 3000, "Distance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomItemLooterSettings.ItemLooterListDistance, 0, 3000);

					LargeButtonToggle("Item Looter Blacklist Enabled###ItemLooterBlacklistToggle", &ErectusIni::CustomItemLooterSettings.ItemLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("Item Looter Formid List"))
					{
						for (int i = 0; i < 100; i++)
						{
							char ItemLooterEnabledText[sizeof("Item Looter Slot: 99")];
							char ItemLooterLabelText[sizeof("###ItemLooterList99")];
							sprintf_s(ItemLooterEnabledText, "Item Looter Slot: %d", i);
							sprintf_s(ItemLooterLabelText, "###ItemLooterList%d", i);
							ButtonToggle(ItemLooterEnabledText, &ErectusIni::CustomItemLooterSettings.ItemLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::CustomItemLooterSettings.ItemLooterFormidList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(ItemLooterLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::CustomItemLooterSettings.ItemLooterFormidList[i]);
						}
					}

					if (ImGui::CollapsingHeader("Item Looter Blacklist"))
					{
						for (int i = 0; i < 64; i++)
						{
							char ItemLooterBlacklistEnabledText[sizeof("Item Looter Blacklist: 63")];
							char ItemLooterBlacklistLabelText[sizeof("###ItemLooterBlacklist63")];
							sprintf_s(ItemLooterBlacklistEnabledText, "Item Looter Blacklist: %d", i);
							sprintf_s(ItemLooterBlacklistLabelText, "###ItemLooterBlacklist%d", i);
							ButtonToggle(ItemLooterBlacklistEnabledText, &ErectusIni::CustomItemLooterSettings.ItemLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::CustomItemLooterSettings.ItemLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(ItemLooterBlacklistLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::CustomItemLooterSettings.ItemLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("NPC Looter (76m Distance Limit)"))
				{
					LargeButtonToggle("Automatic NPC Looting Enabled (Keybind: CTRL+COMMA)###NPCLooterEnabled", &ErectusIni::NPCLooterSettings.EntityLooterEnabled);

					LargeButtonToggle("Draw NPC Looter Status###NPCLooterStatusEnabled", &ErectusIni::NPCLooterSettings.EntityLooterStatusEnabled);

					ButtonToggle("All Weapons Enabled###NPCLooterAllWeaponsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterAllWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###NPCLooterAllArmorEnabled", &ErectusIni::NPCLooterSettings.EntityLooterAllArmorEnabled);

					ButtonToggle("1* Weapons Enabled###NPCLooterOneStarWeaponsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterOneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###NPCLooterOneStarArmorEnabled", &ErectusIni::NPCLooterSettings.EntityLooterOneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###NPCLooterTwoStarWeaponsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterTwoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###NPCLooterTwoStarArmorEnabled", &ErectusIni::NPCLooterSettings.EntityLooterTwoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###NPCLooterThreeStarWeaponsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterThreeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###NPCLooterThreeStarArmorEnabled", &ErectusIni::NPCLooterSettings.EntityLooterThreeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###NPCLooterAmmoEnabled", &ErectusIni::NPCLooterSettings.EntityLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###NPCLooterModsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterModsEnabled);

					ButtonToggle("Caps Enabled###NPCLooterCapsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterCapsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###NPCLooterJunkEnabled", &ErectusIni::NPCLooterSettings.EntityLooterJunkEnabled);

					ButtonToggle("Aid Enabled###NPCLooterAidEnabled", &ErectusIni::NPCLooterSettings.EntityLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###NPCLooterTreasureMapsEnabled", &ErectusIni::NPCLooterSettings.EntityLooterTreasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###NPCLooterKnownPlansEnabled", &ErectusIni::NPCLooterSettings.EntityLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###NPCLooterUnknownPlansEnabled", &ErectusIni::NPCLooterSettings.EntityLooterUnknownPlansEnabled);

					ButtonToggle("Misc Enabled###NPCLooterMiscEnabled", &ErectusIni::NPCLooterSettings.EntityLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###NPCLooterUnlistedEnabled", &ErectusIni::NPCLooterSettings.EntityLooterUnlistedEnabled);

					LargeButtonToggle("NPC Looter Formid List Enabled###NPCLooterListEnabled", &ErectusIni::NPCLooterSettings.EntityLooterListEnabled);

					LargeButtonToggle("NPC Looter Blacklist Enabled###NPCLooterBlacklistToggle", &ErectusIni::NPCLooterSettings.EntityLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("NPC Looter Formid List"))
					{
						for (int i = 0; i < 100; i++)
						{
							char NPCLooterEnabledText[sizeof("NPC Looter Slot: 99")];
							char NPCLooterLabelText[sizeof("###NPCLooterList99")];
							sprintf_s(NPCLooterEnabledText, "NPC Looter Slot: %d", i);
							sprintf_s(NPCLooterLabelText, "###NPCLooterList%d", i);
							ButtonToggle(NPCLooterEnabledText, &ErectusIni::NPCLooterSettings.EntityLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::NPCLooterSettings.EntityLooterFormidList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(NPCLooterLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::NPCLooterSettings.EntityLooterFormidList[i]);
						}
					}

					if (ImGui::CollapsingHeader("NPC Looter Blacklist"))
					{
						for (int i = 0; i < 64; i++)
						{
							char NPCLooterBlacklistEnabledText[sizeof("NPC Looter Blacklist: 63")];
							char NPCLooterBlacklistLabelText[sizeof("###NPCLooterBlacklist63")];
							sprintf_s(NPCLooterBlacklistEnabledText, "NPC Looter Blacklist: %d", i);
							sprintf_s(NPCLooterBlacklistLabelText, "###NPCLooterBlacklist%d", i);
							ButtonToggle(NPCLooterBlacklistEnabledText, &ErectusIni::NPCLooterSettings.EntityLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::NPCLooterSettings.EntityLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(NPCLooterBlacklistLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::NPCLooterSettings.EntityLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Container Looter (6m Distance Limit)"))
				{
					LargeButtonToggle("Automatic Container Looting Enabled (Keybind: CTRL+PERIOD)###ContainerLooterEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterEnabled);

					LargeButtonToggle("Draw Container Looter Status###ContainerLooterStatusEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterStatusEnabled);

					ButtonToggle("All Weapons Enabled###ContainerLooterAllWeaponsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterAllWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("All Armor Enabled###ContainerLooterAllArmorEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterAllArmorEnabled);

					ButtonToggle("1* Weapons Enabled###ContainerLooterOneStarWeaponsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterOneStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("1* Armor Enabled###ContainerLooterOneStarArmorEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterOneStarArmorEnabled);

					ButtonToggle("2* Weapons Enabled###ContainerLooterTwoStarWeaponsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterTwoStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("2* Armor Enabled###ContainerLooterTwoStarArmorEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterTwoStarArmorEnabled);

					ButtonToggle("3* Weapons Enabled###ContainerLooterThreeStarWeaponsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterThreeStarWeaponsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("3* Armor Enabled###ContainerLooterThreeStarArmorEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterThreeStarArmorEnabled);

					ButtonToggle("Ammo Enabled###ContainerLooterAmmoEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterAmmoEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Mods Enabled###ContainerLooterModsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterModsEnabled);

					ButtonToggle("Caps Enabled###ContainerLooterCapsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterCapsEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Junk Enabled###ContainerLooterJunkEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterJunkEnabled);

					ButtonToggle("Aid Enabled###ContainerLooterAidEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterAidEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Treasure Maps Enabled###ContainerLooterTreasureMapsEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterTreasureMapsEnabled);

					ButtonToggle("Known Plans Enabled###ContainerLooterKnownPlansEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterKnownPlansEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Unknown Plans Enabled###ContainerLooterUnknownPlansEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterUnknownPlansEnabled);

					ButtonToggle("Misc Enabled###ContainerLooterMiscEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterMiscEnabled);
					ImGui::SameLine(235.0f);
					ButtonToggle("Other Enabled###ContainerLooterUnlistedEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterUnlistedEnabled);

					LargeButtonToggle("Container Looter Formid List Enabled###ContainerLooterListEnabled", &ErectusIni::ContainerLooterSettings.EntityLooterListEnabled);

					LargeButtonToggle("Container Looter Blacklist Enabled###ContainerLooterBlacklistToggle", &ErectusIni::ContainerLooterSettings.EntityLooterBlacklistToggle);

					if (ImGui::CollapsingHeader("Container Looter Formid List"))
					{
						for (int i = 0; i < 100; i++)
						{
							char ContainerLooterEnabledText[sizeof("Container Looter Slot: 99")];
							char ContainerLooterLabelText[sizeof("###ContainerLooterList99")];
							sprintf_s(ContainerLooterEnabledText, "Container Looter Slot: %d", i);
							sprintf_s(ContainerLooterLabelText, "###ContainerLooterList%d", i);
							ButtonToggle(ContainerLooterEnabledText, &ErectusIni::ContainerLooterSettings.EntityLooterEnabledList[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::ContainerLooterSettings.EntityLooterFormidList[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(ContainerLooterLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::ContainerLooterSettings.EntityLooterFormidList[i]);
						}
					}

					if (ImGui::CollapsingHeader("Container Looter Blacklist"))
					{
						for (int i = 0; i < 64; i++)
						{
							char ContainerLooterBlacklistEnabledText[sizeof("Container Looter Blacklist: 63")];
							char ContainerLooterBlacklistLabelText[sizeof("###ContainerLooterBlacklist63")];
							sprintf_s(ContainerLooterBlacklistEnabledText, "Container Looter Blacklist: %d", i);
							sprintf_s(ContainerLooterBlacklistLabelText, "###ContainerLooterBlacklist%d", i);
							ButtonToggle(ContainerLooterBlacklistEnabledText, &ErectusIni::ContainerLooterSettings.EntityLooterBlacklistEnabled[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::ContainerLooterSettings.EntityLooterBlacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(ContainerLooterBlacklistLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::ContainerLooterSettings.EntityLooterBlacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Flora Harvester (6m Distance Limit)"))
				{
					LargeButtonToggle("Automatic Flora Harvesting Enabled (Keybind: CTRL+P])###HarvesterEnabled", &ErectusIni::CustomHarvesterSettings.HarvesterEnabled);

					LargeButtonToggle("Draw Flora Harvester Status###HarvesterStatusEnabled", &ErectusIni::CustomHarvesterSettings.HarvesterStatusEnabled);

					LargeButtonToggle("Flora Harvester ESP Override (Uses Flora ESP Settings)", &ErectusIni::CustomHarvesterSettings.HarvesterOverrideEnabled);

					for (int i = 0; i < 69; i++)
					{
						ButtonToggle(ErectusIni::CustomHarvesterSettings.HarvesterNameList[i], &ErectusIni::CustomHarvesterSettings.HarvesterEnabledList[i]);
						ImGui::SameLine(235.0f);
						char LabelText[sizeof("###HarvesterReadOnly68")];
						sprintf_s(LabelText, "###HarvesterReadOnly%d", i);
						char FormidText[sizeof("00000000")];
						sprintf_s(FormidText, "%08lX", ErectusIni::CustomHarvesterSettings.HarvesterFormidList[i]);
						ImGui::SetNextItemWidth(224.0f);
						ImGui::InputText(LabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_ReadOnly);
						if (ImGui::IsItemActive()) AllowDrag = false;
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Combat###CombatTab"))
			{
				if (ImGui::CollapsingHeader("Weapon Editor"))
				{
					ButtonToggle("No Recoil", &ErectusIni::CustomWeaponSettings.NoRecoil);
					ImGui::SameLine(235.0f);
					ButtonToggle("Infinite Ammo", &ErectusIni::CustomWeaponSettings.InfiniteAmmo);

					ButtonToggle("No Spread", &ErectusIni::CustomWeaponSettings.NoSpread);
					ImGui::SameLine(235.0f);
					ButtonToggle("Instant Reload", &ErectusIni::CustomWeaponSettings.InstantReload);

					ButtonToggle("No Sway", &ErectusIni::CustomWeaponSettings.NoSway);
					ImGui::SameLine(235.0f);
					ButtonToggle("Automatic Flag###WeaponAutomatic", &ErectusIni::CustomWeaponSettings.Automaticflag);

					ButtonToggle("Capacity###WeaponCapacityEnabled", &ErectusIni::CustomWeaponSettings.CapacityEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###WeaponCapacity", &ErectusIni::CustomWeaponSettings.Capacity, 0, 999, "Capacity: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomWeaponSettings.Capacity, 0, 999);

					ButtonToggle("Speed###WeaponSpeedEnabled", &ErectusIni::CustomWeaponSettings.SpeedEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponSpeed", &ErectusIni::CustomWeaponSettings.Speed, 0.0f, 100.0f, "Speed: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomWeaponSettings.Speed, 0.0f, 100.0f);

					ButtonToggle("Reach###WeaponReachEnabled", &ErectusIni::CustomWeaponSettings.ReachEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###WeaponReach", &ErectusIni::CustomWeaponSettings.Reach, 0.0f, 999.0f, "Reach: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomWeaponSettings.Reach, 0.0f, 999.0f);
				}

				if (ImGui::CollapsingHeader("Targeting Settings"))
				{
					ButtonToggle("Player Targeting (Keybind: T)", &ErectusIni::CustomTargetSettings.LockPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("NPC Targeting (Keybind: T)", &ErectusIni::CustomTargetSettings.LockNPCs);

					ButtonToggle("Damage Redirection (Players)", &ErectusIni::CustomTargetSettings.IndirectPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("Damage Redirection (NPCs)", &ErectusIni::CustomTargetSettings.IndirectNPCs);

					ButtonToggle("Send Damage (Players)", &ErectusIni::CustomTargetSettings.DirectPlayers);
					ImGui::SameLine(235.0f);
					ButtonToggle("Send Damage (NPCs)", &ErectusIni::CustomTargetSettings.DirectNPCs);

					SmallButtonToggle("Living###TargetLiving", &ErectusIni::CustomTargetSettings.TargetLiving);
					ImGui::SameLine(122.0f);
					SmallButtonToggle("Downed###TargetDowned", &ErectusIni::CustomTargetSettings.TargetDowned);
					ImGui::SameLine(235.0f);
					SmallButtonToggle("Dead###TargetDead", &ErectusIni::CustomTargetSettings.TargetDead);
					ImGui::SameLine(349.0f);
					SmallButtonToggle("Unknown###TargetUnknown", &ErectusIni::CustomTargetSettings.TargetUnknown);

					ButtonToggle("Ignore Render Distance###IgnoreRenderDistance", &ErectusIni::CustomTargetSettings.IgnoreRenderDistance);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###TargetLockingFOV", &ErectusIni::CustomTargetSettings.LockingFOV, 5.0f, 40.0f, "Targeting FOV: %.2f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomTargetSettings.LockingFOV, 5.0f, 40.0f);

					ButtonToggle("Ignore Essential NPCs###IgnoreEssentialNPCs", &ErectusIni::CustomTargetSettings.IgnoreEssentialNPCs);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::ColorEdit3("###TargetLockingColor", ErectusIni::CustomTargetSettings.LockingColor);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateRGB(ErectusIni::PlayerSettings.UnknownColor);

					ButtonToggle("Automatic Retargeting###TargetLockingRetargeting", &ErectusIni::CustomTargetSettings.Retargeting);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char TargetLockingCooldownText[sizeof("Cooldown: 120 (1920 ms)")];
					sprintf_s(TargetLockingCooldownText, "Cooldown: %d (%d ms)", ErectusIni::CustomTargetSettings.Cooldown, ErectusIni::CustomTargetSettings.Cooldown * 16);
					ImGui::SliderInt("###TargetLockingCooldown", &ErectusIni::CustomTargetSettings.Cooldown, 0, 120, TargetLockingCooldownText);
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.Cooldown, 0, 120);

					ImGui::SetNextItemWidth(224.0f);
					char SendDamageMinText[sizeof("Send Damage (Min): 60 (960 ms)")];
					sprintf_s(SendDamageMinText, "Send Damage (Min): %d (%d ms)", ErectusIni::CustomTargetSettings.SendDamageMin, ErectusIni::CustomTargetSettings.SendDamageMin * 16);
					ImGui::SliderInt("###SendDamageMin", &ErectusIni::CustomTargetSettings.SendDamageMin, 1, 60, SendDamageMinText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomTargetSettings.SendDamageMax < ErectusIni::CustomTargetSettings.SendDamageMin)
						{
							ErectusIni::CustomTargetSettings.SendDamageMax = ErectusIni::CustomTargetSettings.SendDamageMin;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.SendDamageMin, 1, 60);
					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.SendDamageMax, 1, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char SendDamageMaxText[sizeof("Send Damage (Max): 60 (960 ms)")];
					sprintf_s(SendDamageMaxText, "Send Damage (Max): %d (%d ms)", ErectusIni::CustomTargetSettings.SendDamageMax, ErectusIni::CustomTargetSettings.SendDamageMax * 16);
					ImGui::SliderInt("###SendDamageMax", &ErectusIni::CustomTargetSettings.SendDamageMax, 1, 60, SendDamageMaxText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomTargetSettings.SendDamageMax < ErectusIni::CustomTargetSettings.SendDamageMin)
						{
							ErectusIni::CustomTargetSettings.SendDamageMin = ErectusIni::CustomTargetSettings.SendDamageMax;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.SendDamageMin, 1, 60);
					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.SendDamageMax, 1, 60);
					if (ErectusIni::CustomTargetSettings.SendDamageMax < ErectusIni::CustomTargetSettings.SendDamageMin)
					{
						ErectusIni::CustomTargetSettings.SendDamageMax = ErectusIni::CustomTargetSettings.SendDamageMin;
					}

					char *FavoritedWeaponsPreview = nullptr;
					if (ErectusIni::CustomTargetSettings.FavoriteIndex < 12)
					{
						FavoritedWeaponsPreview = ErectusMemory::GetFavoritedWeaponText(BYTE(ErectusIni::CustomTargetSettings.FavoriteIndex));
						if (FavoritedWeaponsPreview == nullptr)
						{
							FavoritedWeaponsPreview = new char[sizeof("[?] Favorited Item InErectus::Valid")];
							sprintf_s(FavoritedWeaponsPreview, sizeof("[?] Favorited Item InErectus::Valid"), "[%c] Favorited Item InErectus::Valid", ErectusMemory::GetFavoriteSlot(BYTE(ErectusIni::CustomTargetSettings.FavoriteIndex)));
						}
					}
					else
					{
						FavoritedWeaponsPreview = new char[sizeof("[?] No Weapon Selected")];
						sprintf_s(FavoritedWeaponsPreview, sizeof("[?] No Weapon Selected"), "[?] No Weapon Selected");
					}

					ImGui::SetNextItemWidth(451.0f);
					if (ImGui::BeginCombo("###BeginTempCombo", FavoritedWeaponsPreview))
					{
						FavoritedWeaponsArray = ErectusMemory::GetFavoritedWeapons();
						if (FavoritedWeaponsArray == nullptr)
						{
							FavoritedWeaponsArray = new char*[13];
							FavoritedWeaponsArray[0] = new char[sizeof("[?] No Weapon Selected")];
							sprintf_s(FavoritedWeaponsArray[0], sizeof("[?] No Weapon Selected"), "[?] No Weapon Selected");
							for (int i = 1; i < 13; i++)
							{
								FavoritedWeaponsArray[i] = new char[sizeof("[?] Favorited Item InErectus::Valid")];
								sprintf_s(FavoritedWeaponsArray[i], sizeof("[?] Favorited Item InErectus::Valid"), "[%c] Favorited Item InErectus::Valid", ErectusMemory::GetFavoriteSlot(BYTE(i - 1)));
							}
						}

						for (int i = 0; i < 13; i++)
						{
							if (ImGui::Selectable(FavoritedWeaponsArray[i]))
							{
								if (i)
								{
									ErectusIni::CustomTargetSettings.FavoriteIndex = i - 1;
								}
								else
								{
									ErectusIni::CustomTargetSettings.FavoriteIndex = 12;
								}
							}
						}

						ImGui::EndCombo();
						AllowDrag = false;
					}

					if (FavoritedWeaponsPreview != nullptr)
					{
						delete[]FavoritedWeaponsPreview;
						FavoritedWeaponsPreview = nullptr;
					}

					if (FavoritedWeaponsArray != nullptr)
					{
						for (int i = 0; i < 13; i++)
						{
							if (FavoritedWeaponsArray[i] != nullptr)
							{
								delete[]FavoritedWeaponsArray[i];
								FavoritedWeaponsArray[i] = nullptr;
							}
						}

						delete[]FavoritedWeaponsArray;
						FavoritedWeaponsArray = nullptr;
					}

					Erectus::ValidateInt(&ErectusIni::CustomTargetSettings.FavoriteIndex, 0, 12);
				}

				if (ImGui::CollapsingHeader("Melee Settings"))
				{
					LargeButtonToggle("Melee Enabled (Keybind: U)", &ErectusIni::CustomMeleeSettings.MeleeEnabled);

					ImGui::SetNextItemWidth(224.0f);
					char MeleeSpeedMinText[sizeof("Melee Speed (Min): 60 (960 ms)")];
					sprintf_s(MeleeSpeedMinText, "Melee Speed (Min): %d (%d ms)", ErectusIni::CustomMeleeSettings.MeleeSpeedMin, ErectusIni::CustomMeleeSettings.MeleeSpeedMin * 16);
					ImGui::SliderInt("###MeleeSpeedMin", &ErectusIni::CustomMeleeSettings.MeleeSpeedMin, 1, 60, MeleeSpeedMinText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomMeleeSettings.MeleeSpeedMax < ErectusIni::CustomMeleeSettings.MeleeSpeedMin)
						{
							ErectusIni::CustomMeleeSettings.MeleeSpeedMax = ErectusIni::CustomMeleeSettings.MeleeSpeedMin;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomMeleeSettings.MeleeSpeedMin, 1, 60);
					Erectus::ValidateInt(&ErectusIni::CustomMeleeSettings.MeleeSpeedMax, 1, 60);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					char MeleeSpeedMaxText[sizeof("Melee Speed (Max): 60 (960 ms)")];
					sprintf_s(MeleeSpeedMaxText, "Melee Speed (Max): %d (%d ms)", ErectusIni::CustomMeleeSettings.MeleeSpeedMax, ErectusIni::CustomMeleeSettings.MeleeSpeedMax * 16);
					ImGui::SliderInt("###MeleeSpeedMax", &ErectusIni::CustomMeleeSettings.MeleeSpeedMax, 1, 60, MeleeSpeedMaxText);
					if (ImGui::IsItemActive())
					{
						AllowDrag = false;
						if (ErectusIni::CustomMeleeSettings.MeleeSpeedMax < ErectusIni::CustomMeleeSettings.MeleeSpeedMin)
						{
							ErectusIni::CustomMeleeSettings.MeleeSpeedMin = ErectusIni::CustomMeleeSettings.MeleeSpeedMax;
						}
					}
					Erectus::ValidateInt(&ErectusIni::CustomMeleeSettings.MeleeSpeedMin, 1, 60);
					Erectus::ValidateInt(&ErectusIni::CustomMeleeSettings.MeleeSpeedMax, 1, 60);
					if (ErectusIni::CustomMeleeSettings.MeleeSpeedMax < ErectusIni::CustomMeleeSettings.MeleeSpeedMin)
					{
						ErectusIni::CustomMeleeSettings.MeleeSpeedMax = ErectusIni::CustomMeleeSettings.MeleeSpeedMin;
					}
				}

				if (ImGui::CollapsingHeader("One Position Kill"))
				{
					LargeButtonToggle("OPK Players (Keybind: CTRL+B)", &ErectusIni::CustomOpkSettings.PlayersEnabled);
					LargeButtonToggle("OPK NPCs (Keybind: CTRL+N)", &ErectusIni::CustomOpkSettings.NpcsEnabled);
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Player###PlayerTab"))
			{
				if (ImGui::CollapsingHeader("Local Player Settings"))
				{
					LargeButtonToggle("Position Spoofing (Keybind CTRL+L)##LocalPlayerPositionSpoofingEnabled", &ErectusIni::CustomLocalPlayerSettings.PositionSpoofingEnabled);
					ButtonToggle("Draw Position Status###LocalPlayerDrawPositionSpoofingEnabled", &ErectusIni::CustomLocalPlayerSettings.DrawPositionSpoofingEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerPositionSpoofingHeight", &ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight, -524287, 524287, "Spoofed Height: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight, -524287, 524287);

					ButtonToggle("Noclip (Keybind CTRL+Y)###NoclipEnabled", &ErectusIni::CustomLocalPlayerSettings.NoclipEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderFloat("###NoclipSpeed", &ErectusIni::CustomLocalPlayerSettings.NoclipSpeed, 0.0f, 2.0f, "Speed: %.5f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomLocalPlayerSettings.NoclipSpeed, 0.0f, 2.0f);

					ButtonToggle("Client State", &ErectusIni::CustomLocalPlayerSettings.ClientState);
					ImGui::SameLine(235.0f);
					ButtonToggle("Automatic Client State", &ErectusIni::CustomLocalPlayerSettings.AutomaticClientState);

					LargeButtonToggle("Freeze Action Points###LocalPlayerFreezeApEnabled", &ErectusIni::CustomLocalPlayerSettings.FreezeApEnabled);

					ButtonToggle("Action Points###LocalPlayerAPEnabled", &ErectusIni::CustomLocalPlayerSettings.ActionPointsEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerAP", &ErectusIni::CustomLocalPlayerSettings.ActionPoints, 0, 99999, "Action Points: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.ActionPoints, 0, 99999);

					ButtonToggle("Strength###LocalPlayerStrengthEnabled", &ErectusIni::CustomLocalPlayerSettings.StrengthEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerStrength", &ErectusIni::CustomLocalPlayerSettings.Strength, 0, 99999, "Strength: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Strength, 0, 99999);

					ButtonToggle("Perception###LocalPlayerPerceptionEnabled", &ErectusIni::CustomLocalPlayerSettings.PerceptionEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerPerception", &ErectusIni::CustomLocalPlayerSettings.Perception, 0, 99999, "Perception: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Perception, 0, 99999);

					ButtonToggle("Endurance###LocalPlayerEnduranceEnabled", &ErectusIni::CustomLocalPlayerSettings.EnduranceEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerEndurance", &ErectusIni::CustomLocalPlayerSettings.Endurance, 0, 99999, "Endurance: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Endurance, 0, 99999);

					ButtonToggle("Charisma###LocalPlayerCharismaEnabled", &ErectusIni::CustomLocalPlayerSettings.CharismaEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerCharisma", &ErectusIni::CustomLocalPlayerSettings.Charisma, 0, 99999, "Charisma: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Charisma, 0, 99999);

					ButtonToggle("Intelligence###LocalPlayerIntelligenceEnabled", &ErectusIni::CustomLocalPlayerSettings.IntelligenceEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerIntelligence", &ErectusIni::CustomLocalPlayerSettings.Intelligence, 0, 99999, "Intelligence: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Intelligence, 0, 99999);

					ButtonToggle("Agility###LocalPlayerAgilityEnabled", &ErectusIni::CustomLocalPlayerSettings.AgilityEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerAgility", &ErectusIni::CustomLocalPlayerSettings.Agility, 0, 99999, "Agility: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Agility, 0, 99999);

					ButtonToggle("Luck###LocalPlayerLuckEnabled", &ErectusIni::CustomLocalPlayerSettings.LuckEnabled);
					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::SliderInt("###LocalPlayerLuck", &ErectusIni::CustomLocalPlayerSettings.Luck, 0, 99999, "Luck: %d");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateInt(&ErectusIni::CustomLocalPlayerSettings.Luck, 0, 99999);
				}

				if (ImGui::CollapsingHeader("Character Settings"))
				{
					LargeButtonToggle("Character Appearance Editing Enabled###ChargenEditingEnabled", &ErectusIni::CustomChargenSettings.ChargenEditingEnabled);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenThin", &ErectusIni::CustomChargenSettings.Thin, 0.0f, 1.0f, "Character Appearance (Thin): %f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomChargenSettings.Thin, 0.0f, 1.0f);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenMuscular", &ErectusIni::CustomChargenSettings.Muscular, 0.0f, 1.0f, "Character Appearance (Muscular): %f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomChargenSettings.Muscular, 0.0f, 1.0f);

					ImGui::SetNextItemWidth(451.0f);
					ImGui::SliderFloat("###ChargenLarge", &ErectusIni::CustomChargenSettings.Large, 0.0f, 1.0f, "Character Appearance (Large): %f");
					if (ImGui::IsItemActive()) AllowDrag = false;
					Erectus::ValidateFloat(&ErectusIni::CustomChargenSettings.Large, 0.0f, 1.0f);
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Utility###UtilityTab"))
			{
				if (ImGui::CollapsingHeader("Utility"))
				{
					ButtonToggle("Draw Local Player Data", &ErectusIni::CustomUtilitySettings.DebugPlayer);
					ImGui::SameLine(235.0f);
					ButtonToggle("ESP Debug Mode", &ErectusIni::CustomUtilitySettings.DebugEsp);

					if (ErectusIni::CustomUtilitySettings.PtrFormid)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Get Pointer###GetPointerEnabled", ImVec2(224.0f, 0.0f)))
						{
							GetPtrResult = ErectusMemory::GetPtr(ErectusIni::CustomUtilitySettings.PtrFormid);
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
					char PtrFormidText[sizeof("00000000")];
					sprintf_s(PtrFormidText, "%08lX", ErectusIni::CustomUtilitySettings.PtrFormid);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###PtrFormidText", PtrFormidText, sizeof(PtrFormidText), ImGuiInputTextFlags_CharsHexadecimal))
					{
						GetPtrResult = 0;
					}
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(PtrFormidText, "%08lX", &ErectusIni::CustomUtilitySettings.PtrFormid);

					ImGui::SameLine(318.0f);
					char PtrPointerText[sizeof("0000000000000000")];
					sprintf_s(PtrPointerText, "%016llX", GetPtrResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###PtrPointerText", PtrPointerText, sizeof(PtrPointerText), ImGuiInputTextFlags_ReadOnly);
					if (ImGui::IsItemActive()) AllowDrag = false;

					if (ErectusIni::CustomUtilitySettings.AddressFormid)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Get Address###GetAddressEnabled", ImVec2(224.0f, 0.0f)))
						{
							GetAddressResult = ErectusMemory::GetAddress(ErectusIni::CustomUtilitySettings.AddressFormid);
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
					char AddressFormidText[sizeof("00000000")];
					sprintf_s(AddressFormidText, "%08lX", ErectusIni::CustomUtilitySettings.AddressFormid);
					ImGui::SetNextItemWidth(80.0f);
					if (ImGui::InputText("###AddressFormidText", AddressFormidText, sizeof(AddressFormidText), ImGuiInputTextFlags_CharsHexadecimal))
					{
						GetAddressResult = 0;
					}
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(AddressFormidText, "%08lX", &ErectusIni::CustomUtilitySettings.AddressFormid);

					ImGui::SameLine(318.0f);
					char AddressPointerText[sizeof("0000000000000000")];
					sprintf_s(AddressPointerText, "%016llX", GetAddressResult);
					ImGui::SetNextItemWidth(141.0f);
					ImGui::InputText("###AddressPointerText", AddressPointerText, sizeof(AddressPointerText), ImGuiInputTextFlags_ReadOnly);
					if (ImGui::IsItemActive()) AllowDrag = false;
				}

				if (ImGui::CollapsingHeader("Reference Editor"))
				{
					ButtonToggle("Source Formid###SwapperSourceFormidToggle", &SwapperSourceToggle);
					ImGui::SameLine(235.0f);
					char SourceFormidText[sizeof("00000000")];
					sprintf_s(SourceFormidText, "%08lX", ErectusIni::CustomSwapperSettings.SourceFormid);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperSourceFormidText", SourceFormidText, sizeof(SourceFormidText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(SourceFormidText, "%08lX", &ErectusIni::CustomSwapperSettings.SourceFormid);

					ButtonToggle("Destination Formid###SwapperDestinationFormidToggle", &SwapperDestinationToggle);
					ImGui::SameLine(235.0f);
					char DestinationFormidText[sizeof("00000000")];
					sprintf_s(DestinationFormidText, "%08lX", ErectusIni::CustomSwapperSettings.DestinationFormid);
					ImGui::SetNextItemWidth(224.0f);
					ImGui::InputText("###SwapperDestinationFormidText", DestinationFormidText, sizeof(DestinationFormidText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(DestinationFormidText, "%08lX", &ErectusIni::CustomSwapperSettings.DestinationFormid);

					if (SwapperSourceToggle && ErectusIni::CustomSwapperSettings.SourceFormid && SwapperDestinationToggle && ErectusIni::CustomSwapperSettings.DestinationFormid)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceEnabled", ImVec2(451.0f, 0.0f)))
						{
							if (ErectusMemory::ReferenceSwap(&ErectusIni::CustomSwapperSettings.SourceFormid, &ErectusIni::CustomSwapperSettings.DestinationFormid))
							{
								ErectusIni::CustomSwapperSettings.DestinationFormid = ErectusIni::CustomSwapperSettings.SourceFormid;
								SwapperSourceToggle = false;
								SwapperDestinationToggle = false;
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
					SmallButtonToggle("Source###TransferSourceFormidToggle", &TransferSourceToggle);

					ImGui::SameLine(122.0f);
					char SourceFormidText[sizeof("00000000")];
					sprintf_s(SourceFormidText, "%08lX", ErectusIni::CustomTransferSettings.SourceFormid);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferSourceFormidText", SourceFormidText, sizeof(SourceFormidText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(SourceFormidText, "%08lX", &ErectusIni::CustomTransferSettings.SourceFormid);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferSourceLocalPlayer", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::CustomTransferSettings.SourceFormid = ErectusMemory::GetLocalPlayerFormid();
					}
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferSourceSTASH", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::CustomTransferSettings.SourceFormid = ErectusMemory::GetStashFormid();
					}
					ImGui::PopStyleColor(3);

					SmallButtonToggle("Destination###TransferDestinationFormidToggle", &TransferDestinationToggle);
					ImGui::SameLine(122.0f);
					char DestinationFormidText[sizeof("00000000")];
					sprintf_s(DestinationFormidText, "%08lX", ErectusIni::CustomTransferSettings.DestinationFormid);
					ImGui::SetNextItemWidth(110.0f);
					ImGui::InputText("###TransferDestinationFormidText", DestinationFormidText, sizeof(DestinationFormidText), ImGuiInputTextFlags_CharsHexadecimal);
					if (ImGui::IsItemActive()) AllowDrag = false;
					sscanf_s(DestinationFormidText, "%08lX", &ErectusIni::CustomTransferSettings.DestinationFormid);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					ImGui::SameLine(235.0f);
					if (ImGui::Button("Get Player###TransferDestinationLocalPlayer", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::CustomTransferSettings.DestinationFormid = ErectusMemory::GetLocalPlayerFormid();
					}
					ImGui::SameLine(349.0f);
					if (ImGui::Button("Get STASH###TransferDestinationSTASH", ImVec2(110.0f, 0.0f)))
					{
						ErectusIni::CustomTransferSettings.DestinationFormid = ErectusMemory::GetStashFormid();
					}
					ImGui::PopStyleColor(3);

					bool AllowTransfer = false;

					if (TransferSourceToggle && ErectusIni::CustomTransferSettings.SourceFormid && TransferDestinationToggle && ErectusIni::CustomTransferSettings.DestinationFormid)
					{
						if (ErectusIni::CustomTransferSettings.UseWhitelist)
						{
							AllowTransfer = ErectusMemory::CheckItemTransferList();
						}
						else
						{
							AllowTransfer = true;
						}
					}

					if (AllowTransfer)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
						if (ImGui::Button("Transfer Items###TransferItemsEnabled", ImVec2(451.0f, 0.0f)))
						{
							ErectusMemory::TransferItems(ErectusIni::CustomTransferSettings.SourceFormid, ErectusIni::CustomTransferSettings.DestinationFormid);
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

					LargeButtonToggle("Use Item Transfer Whitelist", &ErectusIni::CustomTransferSettings.UseWhitelist);
					LargeButtonToggle("Use Item Transfer Blacklist", &ErectusIni::CustomTransferSettings.UseBlacklist);

					if (ImGui::CollapsingHeader("Item Transfer Whitelist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char WhitelistedText[sizeof("Transfer Whitelist Slot: 31")];
							char WhitelistText[sizeof("###ItemTransferWhitelist31")];
							sprintf_s(WhitelistedText, "Transfer Whitelist Slot: %d", i);
							sprintf_s(WhitelistText, "###ItemTransferWhitelist%d", i);
							ButtonToggle(WhitelistedText, &ErectusIni::CustomTransferSettings.Whitelisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::CustomTransferSettings.Whitelist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(WhitelistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::CustomTransferSettings.Whitelist[i]);
						}
					}

					if (ImGui::CollapsingHeader("Item Transfer Blacklist Settings"))
					{
						for (int i = 0; i < 32; i++)
						{
							char BlacklistedText[sizeof("Transfer Blacklist Slot: 31")];
							char BlacklistText[sizeof("###ItemTransferBlacklist31")];
							sprintf_s(BlacklistedText, "Transfer Blacklist Slot: %d", i);
							sprintf_s(BlacklistText, "###ItemTransferBlacklist%d", i);
							ButtonToggle(BlacklistedText, &ErectusIni::CustomTransferSettings.Blacklisted[i]);
							ImGui::SameLine(235.0f);
							char FormidText[sizeof("00000000")];
							sprintf_s(FormidText, "%08lX", ErectusIni::CustomTransferSettings.Blacklist[i]);
							ImGui::SetNextItemWidth(224.0f);
							ImGui::InputText(BlacklistText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
							if (ImGui::IsItemActive()) AllowDrag = false;
							sscanf_s(FormidText, "%08lX", &ErectusIni::CustomTransferSettings.Blacklist[i]);
						}
					}
				}

				if (ImGui::CollapsingHeader("Nuke Codes"))
				{
					ButtonToggle("Automatic Nuke Codes", &ErectusIni::CustomNukeCodeSettings.AutomaticNukeCodes);
					ImGui::SameLine(235.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
					if (ImGui::Button("Get Nuke Codes", ImVec2(224.0f, 0.0f)))
					{
						ErectusMemory::GetNukeCode(0x000921AE, AlphaCode);
						ErectusMemory::GetNukeCode(0x00092213, BravoCode);
						ErectusMemory::GetNukeCode(0x00092214, CharlieCode);
					}
					ImGui::PopStyleColor(3);

					ButtonToggle("Draw Nuke Code Alpha", &ErectusIni::CustomNukeCodeSettings.DrawCodeAlpha);
					ImGui::SameLine(255.0f);
					ImGui::Text("%d %d %d %d %d %d %d %d - Alpha", AlphaCode[0], AlphaCode[1], AlphaCode[2], AlphaCode[3], AlphaCode[4], AlphaCode[5], AlphaCode[6], AlphaCode[7]);

					ButtonToggle("Draw Nuke Code Bravo", &ErectusIni::CustomNukeCodeSettings.DrawCodeBravo);
					ImGui::SameLine(255.0f);
					ImGui::Text("%d %d %d %d %d %d %d %d - Bravo", BravoCode[0], BravoCode[1], BravoCode[2], BravoCode[3], BravoCode[4], BravoCode[5], BravoCode[6], BravoCode[7]);

					ButtonToggle("Draw Nuke Code Charlie", &ErectusIni::CustomNukeCodeSettings.DrawCodeCharlie);
					ImGui::SameLine(255.0f);
					ImGui::Text("%d %d %d %d %d %d %d %d - Charlie", CharlieCode[0], CharlieCode[1], CharlieCode[2], CharlieCode[3], CharlieCode[4], CharlieCode[5], CharlieCode[6], CharlieCode[7]);
				}
				
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Teleporter###TeleporterTab"))
			{
				for (int i = 0; i < 16; i++)
				{
					char TeleportHeaderText[sizeof("Teleporter Slot: 15")];
					sprintf_s(TeleportHeaderText, "Teleport Slot: %d", i);
					if (ImGui::CollapsingHeader(TeleportHeaderText))
					{
						ImGui::SetNextItemWidth(110.0f);
						char TeleportDestinationTextX[sizeof("###TeleportDestinationX15")];
						sprintf_s(TeleportDestinationTextX, "###TeleportDestinationX%d", i);
						ImGui::InputFloat(TeleportDestinationTextX, &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].Destination[0]);
						if (ImGui::IsItemActive()) AllowDrag = false;
						ImGui::SameLine(122.0f);
						ImGui::SetNextItemWidth(110.0f);
						char TeleportDestinationTextY[sizeof("###TeleportDestinationY15")];
						sprintf_s(TeleportDestinationTextY, "###TeleportDestinationY%d", i);
						ImGui::InputFloat(TeleportDestinationTextY, &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].Destination[1]);
						if (ImGui::IsItemActive()) AllowDrag = false;
						ImGui::SameLine(235.0f);
						ImGui::SetNextItemWidth(110.0f);
						char TeleportDestinationTextZ[sizeof("###TeleportDestinationZ15")];
						sprintf_s(TeleportDestinationTextZ, "###TeleportDestinationZ%d", i);
						ImGui::InputFloat(TeleportDestinationTextZ, &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].Destination[2]);
						if (ImGui::IsItemActive()) AllowDrag = false;
						ImGui::SameLine(349.0f);
						ImGui::SetNextItemWidth(110.0f);
						char TeleportDestinationTextW[sizeof("###TeleportDestinationW15")];
						sprintf_s(TeleportDestinationTextW, "###TeleportDestinationW%d", i);
						ImGui::InputFloat(TeleportDestinationTextW, &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].Destination[3]);
						if (ImGui::IsItemActive()) AllowDrag = false;

						char FormidLabelText[sizeof("###TeleportCellFormid15")];
						sprintf_s(FormidLabelText, "###TeleportCellFormid%d", i);
						char FormidText[sizeof("00000000")];
						sprintf_s(FormidText, "%08lX", ErectusIni::CustomTeleportSettings.TeleportEntryData[i].CellFormid);
						ImGui::SetNextItemWidth(110.0f);
						ImGui::InputText(FormidLabelText, FormidText, sizeof(FormidText), ImGuiInputTextFlags_CharsHexadecimal);
						if (ImGui::IsItemActive()) AllowDrag = false;
						sscanf_s(FormidText, "%08lX", &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].CellFormid);
						ImGui::SameLine(122.0f);
						char TeleportDestinationEnabledText[sizeof("Set Position###TeleportDestinationEnabled15")];
						sprintf_s(TeleportDestinationEnabledText, "Set Position###TeleportDestinationEnabled%d", i);
						char TeleportDestinationDisabledText[sizeof("Set Position###TeleportDestinationDisabled15")];
						sprintf_s(TeleportDestinationDisabledText, "Set Position###TeleportDestinationDisabled%d", i);
						if (!ErectusIni::CustomTeleportSettings.TeleportEntryData[i].DisableSaving)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
							if (ImGui::Button(TeleportDestinationEnabledText, ImVec2(110.0f, 0.0f))) ErectusMemory::GetTeleportPosition(i);
							ImGui::PopStyleColor(3);
						}
						else
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
							ImGui::Button(TeleportDestinationDisabledText, ImVec2(110.0f, 0.0f));
							ImGui::PopStyleColor(3);
							ImGui::PopItemFlag();
						}
						ImGui::SameLine(235.0f);
						char DisableSavingText[sizeof("Lock###DisableSaving15")];
						sprintf_s(DisableSavingText, "Lock###DisableSaving%d", i);
						SmallButtonToggle(DisableSavingText, &ErectusIni::CustomTeleportSettings.TeleportEntryData[i].DisableSaving);
						ImGui::SameLine(349.0f);
						char TeleportRequestEnabledText[sizeof("Teleport###TeleportRequestEnabled15")];
						sprintf_s(TeleportRequestEnabledText, "Teleport###TeleportRequestEnabled%d", i);
						char TeleportRequestDisabledText[sizeof("Teleport###TeleportRequestDisabled15")];
						sprintf_s(TeleportRequestDisabledText, "Teleport###TeleportRequestDisabled%d", i);
						if (ErectusIni::CustomTeleportSettings.TeleportEntryData[i].CellFormid)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
							if (ImGui::Button(TeleportRequestEnabledText, ImVec2(110.0f, 0.0f))) ErectusMemory::RequestTeleport(i);
							ImGui::PopStyleColor(3);
						}
						else
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
							ImGui::Button(TeleportRequestDisabledText, ImVec2(110.0f, 0.0f));
							ImGui::PopStyleColor(3);
							ImGui::PopItemFlag();
						}
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("BitMsgWriter###BitMsgWriterTab"))
			{
				LargeButtonToggle("Message Sender Enabled", &ErectusMemory::AllowMessages);
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}

		if (ImGui::GetActiveID() == ImGui::GetWindowScrollbarID(ImGui::GetCurrentWindow(), ImGuiAxis_Y))
		{
			AllowDrag = false;
		}

		if (ImGui::IsMouseDragging(0) && AllowDrag)
		{
			if (!PointerDrag)
			{
				PointerOrigin = ImGui::GetMousePos();
				PointerDrag = true;
			}
		}
		else
		{
			if (PointerDrag)
			{
				PointerOrigin = { 0.0f, 0.0f };
				PointerDrag = false;
			}
		}

		if (PointerDrag)
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
	ImGuiContextCreated = true;

	if (!ImGui_ImplWin32_Init(ErectusMain::WindowHwnd)) return false;
	else ImGuiWin32Initialized = true;

	if (!ImGui_ImplDX9_Init(ErectusD3D9::D3D9Device)) return false;
	else ImGuiD3D9Initialized = true;

	return true;
}

void ErectusImGui::ImGuiCleanup()
{
	if (ImGuiD3D9Initialized)
	{
		ImGui_ImplDX9_Shutdown();
		ImGuiD3D9Initialized = false;
	}

	if (ImGuiWin32Initialized)
	{
		ImGui_ImplWin32_Shutdown();
		ImGuiWin32Initialized = false;
	}

	if (ImGuiContextCreated)
	{
		ImGui::DestroyContext();
		ImGuiContextCreated = false;
	}
}
