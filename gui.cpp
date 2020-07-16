#include "gui.h"

#include "app.h"
#include "common.h"
#include "settings.h"
#include "renderer.h"

#include "ErectusProcess.h"
#include "ErectusMemory.h"
#include "threads.h"
#include "utils.h"

#include "fmt/format.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"


void Gui::Render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ProcessMenu();
	OverlayMenu();
	RenderOverlay();


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Gui::RenderOverlay()
{
	if (!App::overlayActive)
		return;

	Renderer::d3DxSprite->Begin(D3DXSPRITE_ALPHABLEND);

	RenderEntities();
	RenderPlayers();

	RenderInfoBox();

	Renderer::d3DxSprite->End();
}

void Gui::RenderEntities()
{
	auto entities = ErectusMemory::entityDataBuffer;
	for (const auto& entity : entities)
	{
		if (entity.flag & CUSTOM_ENTRY_ENTITY)
			RenderItems(entity, Settings::esp.entities);
		else if (entity.flag & CUSTOM_ENTRY_JUNK)
			RenderItems(entity, Settings::esp.junk);
		else if (entity.flag & CUSTOM_ENTRY_ITEM)
			RenderItems(entity, Settings::esp.items);
		else if (entity.flag & CUSTOM_ENTRY_CONTAINER)
			RenderItems(entity, Settings::esp.containers);
		else if (entity.flag & CUSTOM_ENTRY_PLAN)
			RenderItems(entity, Settings::esp.plans);
		else if (entity.flag & CUSTOM_ENTRY_MAGAZINE)
			RenderItems(entity, Settings::esp.magazines);
		else if (entity.flag & CUSTOM_ENTRY_BOBBLEHEAD)
			RenderItems(entity, Settings::esp.bobbleheads);
		else if (entity.flag & CUSTOM_ENTRY_FLORA)
			RenderItems(entity, Settings::esp.flora);
		else if (entity.flag & CUSTOM_ENTRY_NPC)
			RenderActors(entity, Settings::esp.npcs);
	}
}

void Gui::RenderPlayers()
{
	auto players = ErectusMemory::playerDataBuffer;
	for (const auto& player : players) {
		if (player.flag & CUSTOM_ENTRY_PLAYER)
			RenderActors(player, Settings::esp.players);
	}
}

void Gui::RenderActors(const CustomEntry& entry, const EspSettings::Actors& settings)
{
	auto health = -1;
	BYTE epicRank = 0;
	auto allowNpc = false;
	if (entry.flag & CUSTOM_ENTRY_NPC)
	{
		TesObjectRefr npcData{};
		if (!ErectusProcess::Rpm(entry.entityPtr, &npcData, sizeof npcData))
			return;

		ActorSnapshotComponent actorSnapshotComponentData{};
		if (ErectusMemory::GetActorSnapshotComponentData(npcData, &actorSnapshotComponentData))
		{
			health = static_cast<int>(actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth);
			epicRank = actorSnapshotComponentData.epicRank;
			if (epicRank)
			{
				switch (ErectusMemory::CheckHealthFlag(npcData.healthFlag))
				{
				case 0x01: //Alive
				case 0x02: //Downed
				case 0x03: //Dead
					switch (epicRank)
					{
					case 1:
						allowNpc = Settings::esp.legendaryNpcs.overrideLivingOneStar;
						break;
					case 2:
						allowNpc = Settings::esp.legendaryNpcs.overrideLivingTwoStar;
						break;
					case 3:
						allowNpc = Settings::esp.legendaryNpcs.overrideLivingThreeStar;
						break;
					default:
						break;
					}
				default:
					break;
				}
			}
		}
	}

	if (!settings.enabled && !allowNpc)
		return;

	if (!settings.drawEnabled && !settings.drawDisabled)
		return;

	if (settings.enabledAlpha == 0.0f && settings.disabledAlpha == 0.0f)
		return;

	if (!settings.drawNamed && !settings.drawUnnamed)
		return;

	TesObjectRefr entityData{};
	if (!ErectusProcess::Rpm(entry.entityPtr, &entityData, sizeof entityData))
		return;

	if (entry.flag & CUSTOM_ENTRY_PLAYER)
	{
		ActorSnapshotComponent actorSnapshotComponentData{};
		if (ErectusMemory::GetActorSnapshotComponentData(entityData, &actorSnapshotComponentData))
			health = static_cast<int>(actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth);
	}

	if (entry.flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!settings.drawUnnamed)
			return;
	}
	else
	{
		if (!settings.drawNamed)
			return;
	}

	auto alpha = 0.f;

	if (entityData.spawnFlag == 0x02)
	{
		if (settings.drawEnabled)
			alpha = settings.enabledAlpha;
	}
	else
	{
		if (settings.drawDisabled)
			alpha = settings.disabledAlpha;
	}

	if (alpha == 0.f)
		return;

	auto showHealthText = false;

	const float* color = nullptr;

	auto legendaryAlpha = 1.0f;

	switch (ErectusMemory::CheckHealthFlag(entityData.healthFlag))
	{
	case 0x01: //Alive
		showHealthText = settings.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = Settings::esp.legendaryNpcs.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = Settings::esp.legendaryNpcs.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = Settings::esp.legendaryNpcs.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawAlive)
			color = settings.aliveColor;
		break;
	case 0x02: //Downed
		showHealthText = settings.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = Settings::esp.legendaryNpcs.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = Settings::esp.legendaryNpcs.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = Settings::esp.legendaryNpcs.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawDowned)
			color = settings.downedColor;
		break;
	case 0x03: //Dead
		showHealthText = settings.showDeadHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = Settings::esp.legendaryNpcs.deadOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = Settings::esp.legendaryNpcs.deadTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = Settings::esp.legendaryNpcs.deadThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawDead)
			color = settings.deadColor;
		break;
	default: //Unknown
		showHealthText = settings.showHealth;
		if (settings.drawUnknown)
			color = settings.unknownColor;
		break;
	}

	if (color == nullptr)
		return;

	auto cameraData = ErectusMemory::GetCameraInfo();
	auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > settings.enabledDistance)
		return;

	if (entry.entityPtr == ErectusMemory::targetLockingPtr)
		color = Settings::targetting.lockingColor;

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::WorldToScreen(cameraData.view, entityData.position, screen))
		return;

	std::string itemText;
	if (settings.showName && showHealthText && settings.showDistance) //Name, Health, Distance
		itemText = fmt::format("{0}\n{1:d} hp [{2:d} m]", entry.name, health, normalDistance);
	else if (settings.showName && showHealthText && !settings.showDistance) //Name, Health
		itemText = fmt::format("{0}\n{1:d} hp", entry.name, health);
	else if (settings.showName && !showHealthText && settings.showDistance) //Name, Distance
		itemText = fmt::format("{0}\n[{1:d} m]", entry.name, normalDistance);
	else if (!settings.showName && showHealthText && settings.showDistance) //Health, Distance
		itemText = fmt::format("{0:d} hp [{1:d} m]", health, normalDistance);
	else if (settings.showName && !showHealthText && !settings.showDistance) //Name
		itemText = entry.name;
	else if (!settings.showName && showHealthText && !settings.showDistance) //Health
		itemText = fmt::format("{:d} hp", health);
	else if (!settings.showName && !showHealthText && settings.showDistance) //Distance
		itemText = fmt::format("[{:d} m]", normalDistance);

	if (!itemText.empty())
	{
		if (Settings::utilities.debugEsp)
			itemText = fmt::format("{0:16x}\n{1:08x}\n{2:16x}\n{3:08x}", entry.entityPtr, entry.entityFormId, entry.referencePtr, entry.referenceFormId);

		Renderer::DrawTextA(itemText.c_str(), settings.textShadowed, settings.textCentered, screen, color, alpha);
	}
}

void Gui::RenderItems(const CustomEntry& entry, const EspSettings::Items& settings)
{
	if (!(entry.flag & CUSTOM_ENTRY_WHITELISTED) && !settings.enabled)
		return;

	if (!settings.drawEnabled && !settings.drawDisabled)
		return;

	if (settings.enabledAlpha == 0.0f && settings.disabledAlpha == 0.0f)
		return;

	if (!settings.drawNamed && !settings.drawUnnamed)
		return;

	TesObjectRefr entityData{};
	if (!ErectusProcess::Rpm(entry.entityPtr, &entityData, sizeof entityData))
		return;

	if (entry.flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!settings.drawUnnamed)
			return;
	}
	else if (!settings.drawNamed)
		return;

	if (entry.flag & CUSTOM_ENTRY_PLAN)
	{
		if (!Settings::esp.recipes.knownRecipesEnabled && !Settings::esp.recipes.unknownRecipesEnabled)
			return;

		if (!Settings::esp.recipes.knownRecipesEnabled && entry.flag & CUSTOM_ENTRY_KNOWN_RECIPE)
			return;
		if (!Settings::esp.recipes.unknownRecipesEnabled && entry.flag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
			return;
	}

	auto alpha = 0.f;

	if (entityData.spawnFlag == 0x02)
	{
		if (settings.drawEnabled)
		{
			if (entry.flag & CUSTOM_ENTRY_FLORA)
			{
				if (!ErectusMemory::IsFloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
					alpha = settings.enabledAlpha;
				else if (settings.drawDisabled)
					alpha = settings.disabledAlpha;
			}
			else
				alpha = settings.enabledAlpha;
		}
	}
	else
	{
		if (settings.drawDisabled)
			alpha = settings.disabledAlpha;
	}

	if (alpha == 0.f)
		return;

	auto cameraData = ErectusMemory::GetCameraInfo();

	const auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	const auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > settings.enabledDistance)
		return;

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::WorldToScreen(cameraData.view, entityData.position, screen))
		return;

	std::string itemText{};
	if (settings.showName && settings.showDistance)
		itemText = fmt::format("{0}\n[{1:d} m]", entry.name, normalDistance);
	else if (settings.showName && !settings.showDistance)
		itemText = entry.name;
	else if (!settings.showName && settings.showDistance)
		itemText = fmt::format("[{0:d} m]", normalDistance);

	if (!itemText.empty())
	{
		if (Settings::utilities.debugEsp)
			itemText = fmt::format("{0:16x}\n{1:08x}\n{2:16x}\n{3:08x}", entry.entityPtr, entry.entityFormId, entry.referencePtr, entry.referenceFormId);

		Renderer::DrawTextA(itemText.c_str(), settings.textShadowed, settings.textCentered, screen, settings.color, alpha);
	}
}

void Gui::RenderInfoBox()
{
	std::vector<std::pair<std::string, bool>> infoTexts = {};

	std::string featureText = {};
	auto featureState = false;

	float enabledTextColor[3] = { 0.0f, 1.0f, 0.0f };
	float disabledTextColor[3] = { 1.0f, 0.0f, 0.0f };

	if (Settings::infobox.drawLocalPlayerInfo) {
		auto localPlayer = ErectusMemory::GetLocalPlayerInfo();

		featureText = fmt::format("Player FormId: {:08x}", localPlayer.formId);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("STASH FormId: {:08x}", localPlayer.stashFormId);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Cell FormId: {:08x}", localPlayer.cellFormId);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("X: {:f}", localPlayer.position[0]);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Y: {:f}", localPlayer.position[1]);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Z: {:f}", localPlayer.position[2]);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Yaw: {:f}", localPlayer.yaw);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Pitch: {:f}", localPlayer.pitch);
		infoTexts.emplace_back(featureText, true);

		featureText = fmt::format("Health: {:f}", localPlayer.currentHealth);
		infoTexts.emplace_back(featureText, true);
	}

	if (Settings::infobox.drawPositionSpoofingStatus)
	{
		featureText = fmt::format("Position Spoofing (Active): {0:d} (Height: {1:d})", static_cast<int>(Threads::positionSpoofingToggle), Settings::localPlayer.positionSpoofingHeight);
		featureState = ErectusMemory::InsideInteriorCell() ? false : Settings::localPlayer.positionSpoofingEnabled;
		infoTexts.emplace_back(featureText, featureState);
	}

	if (Settings::infobox.drawNukeCodes)
	{
		featureText = format("{} - Alpha", fmt::join(ErectusMemory::alphaCode, " "));
		featureState = ErectusMemory::alphaCode == std::array<int, 8>{} ? false : true;
		infoTexts.emplace_back(featureText, featureState);

		featureText = format("{} - Bravo", fmt::join(ErectusMemory::bravoCode, " "));
		featureState = ErectusMemory::bravoCode == std::array<int, 8>{} ? false : true;
		infoTexts.emplace_back(featureText, featureState);

		featureText = format("{} - Charlie", fmt::join(ErectusMemory::charlieCode, " "));
		featureState = ErectusMemory::charlieCode == std::array<int, 8>{} ? false : true;
		infoTexts.emplace_back(featureText, featureState);
	}

	auto spacing = 0;
	for (const auto& item : infoTexts)
	{
		float textPosition[2] = { 0.0f, static_cast<float>(spacing) * 16.0f };
		auto* textColor = item.second ? enabledTextColor : disabledTextColor;
		Renderer::DrawTextA(item.first.c_str(), true, false, textPosition, textColor, 1.f);
		spacing++;
	}
}

void Gui::ProcessMenu()
{
	if (!ErectusProcess::processMenuActive)
		return;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(App::windowSize[0]), static_cast<float>(App::windowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	if (ImGui::Begin("Erectus - Process Menu", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
				App::CloseWnd();
			if (ImGui::MenuItem("Overlay Menu"))
				App::SetOverlayMenu();
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
					App::SetOverlayMenu();
				if (ImGui::MenuItem("Overlay"))
					App::SetOverlayPosition(false, true);
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
}

void Gui::ButtonToggle(const char* label, bool& state)
{
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
			state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x / 2, 0.0f)))
			state = true;
		ImGui::PopStyleColor(3);
	}
}

void Gui::LargeButtonToggle(const char* label, bool& state)
{
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x + 1.f, 0)))
			state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x + 1.f, 0)))
			state = true;
		ImGui::PopStyleColor(3);
	}
}

void Gui::SmallButtonToggle(const char* label, bool& state)
{
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f)))
			state = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Button(label, ImVec2(110.0f, 0.0f)))
			state = true;
		ImGui::PopStyleColor(3);
	}
}

void Gui::OverlayMenuTabEsp()
{
	if (ImGui::BeginTabItem("ESP###ESPTab"))
	{
		if (ImGui::CollapsingHeader("Player ESP Settings"))
		{
			ButtonToggle("Player ESP Enabled", Settings::esp.players.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.playersEnabledDistance", &Settings::esp.players.enabledDistance, 0, 3000, "Distance: %d");

			ButtonToggle("Draw Living Players", Settings::esp.players.drawAlive);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.playersAliveColor", Settings::esp.players.aliveColor);
			Utils::ValidateRgb(Settings::esp.players.aliveColor);

			ButtonToggle("Draw Downed Players", Settings::esp.players.drawDowned);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.playersDownedColor", Settings::esp.players.downedColor);
			Utils::ValidateRgb(Settings::esp.players.downedColor);

			ButtonToggle("Draw Dead Players", Settings::esp.players.drawDead);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.playersDeadColor", Settings::esp.players.deadColor);
			Utils::ValidateRgb(Settings::esp.players.deadColor);

			ButtonToggle("Draw Unknown Players", Settings::esp.players.drawUnknown);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.playersUnknownColor", Settings::esp.players.unknownColor);
			Utils::ValidateRgb(Settings::esp.players.unknownColor);

			ButtonToggle("Draw Enabled Players", Settings::esp.players.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.playersEnabledAlpha", &Settings::esp.players.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Players", Settings::esp.players.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.playersDisabledAlpha", &Settings::esp.players.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Players", Settings::esp.players.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Players", Settings::esp.players.drawUnnamed);

			ButtonToggle("Show Player Name", Settings::esp.players.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Player Distance", Settings::esp.players.showDistance);

			ButtonToggle("Show Player Health", Settings::esp.players.showHealth);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Dead Player Health", Settings::esp.players.showDeadHealth);

			ButtonToggle("Player Text Shadowed", Settings::esp.players.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Player Text Centered", Settings::esp.players.textCentered);
		}

		if (ImGui::CollapsingHeader("NPC ESP Settings"))
		{
			ButtonToggle("NPC ESP Enabled", Settings::esp.npcs.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.npcsEnabledDistance", &Settings::esp.npcs.enabledDistance, 0, 3000, "Distance: %d");

			ButtonToggle("Draw Living NPCs", Settings::esp.npcs.drawAlive);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.npcsAliveColor", Settings::esp.npcs.aliveColor);
			Utils::ValidateRgb(Settings::esp.npcs.aliveColor);

			ButtonToggle("Draw Downed NPCs", Settings::esp.npcs.drawDowned);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.npcsDownedColor", Settings::esp.npcs.downedColor);
			Utils::ValidateRgb(Settings::esp.npcs.downedColor);

			ButtonToggle("Draw Dead NPCs", Settings::esp.npcs.drawDead);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.npcsDeadColor", Settings::esp.npcs.deadColor);
			Utils::ValidateRgb(Settings::esp.npcs.deadColor);

			ButtonToggle("Draw Unknown NPCs", Settings::esp.npcs.drawUnknown);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###esp.npcsUnknownColor", Settings::esp.npcs.unknownColor);
			Utils::ValidateRgb(Settings::esp.npcs.unknownColor);

			ButtonToggle("Draw Enabled NPCs", Settings::esp.npcs.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.npcsEnabledAlpha", &Settings::esp.npcs.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled NPCs", Settings::esp.npcs.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.npcsDisabledAlpha", &Settings::esp.npcs.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named NPCs", Settings::esp.npcs.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed NPCs", Settings::esp.npcs.drawUnnamed);

			ButtonToggle("Show NPC Name", Settings::esp.npcs.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show NPC Distance", Settings::esp.npcs.showDistance);

			ButtonToggle("Show NPC Health", Settings::esp.npcs.showHealth);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Dead NPC Health", Settings::esp.npcs.showDeadHealth);

			ButtonToggle("NPC Text Shadowed", Settings::esp.npcs.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("NPC Text Centered", Settings::esp.npcs.textCentered);

			ButtonToggle("Always Draw Living 1* NPCs", Settings::esp.legendaryNpcs.overrideLivingOneStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###LivingOneStarColor", Settings::esp.legendaryNpcs.livingOneStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.livingOneStarColor);

			ButtonToggle("Always Draw Dead 1* NPCs", Settings::esp.legendaryNpcs.overrideDeadOneStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###DeadOneStarColor", Settings::esp.legendaryNpcs.deadOneStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.deadOneStarColor);

			ButtonToggle("Always Draw Living 2* NPCs", Settings::esp.legendaryNpcs.overrideLivingTwoStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###LivingTwoStarColor", Settings::esp.legendaryNpcs.livingTwoStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.livingTwoStarColor);

			ButtonToggle("Always Draw Dead 2* NPCs", Settings::esp.legendaryNpcs.overrideDeadTwoStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###DeadTwoStarColor", Settings::esp.legendaryNpcs.deadTwoStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.deadTwoStarColor);

			ButtonToggle("Always Draw Living 3* NPCs", Settings::esp.legendaryNpcs.overrideLivingThreeStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###LivingThreeStarColor", Settings::esp.legendaryNpcs.livingThreeStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.livingThreeStarColor);

			ButtonToggle("Always Draw Dead 3* NPCs", Settings::esp.legendaryNpcs.overrideDeadThreeStar);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###DeadThreeStarColor", Settings::esp.legendaryNpcs.deadThreeStarColor);
			Utils::ValidateRgb(Settings::esp.legendaryNpcs.deadThreeStarColor);
		}

		if (ImGui::CollapsingHeader("Container ESP Settings"))
		{
			ButtonToggle("Container ESP Enabled", Settings::esp.containers.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.containersEnabledDistance", &Settings::esp.containers.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.containersColor", Settings::esp.containers.color);
			Utils::ValidateRgb(Settings::esp.containers.color);

			ButtonToggle("Draw Enabled Containers", Settings::esp.containers.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.containersEnabledAlpha", &Settings::esp.containers.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Containers", Settings::esp.containers.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.containersDisabledAlpha", &Settings::esp.containers.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Containers", Settings::esp.containers.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Containers", Settings::esp.containers.drawUnnamed);

			ButtonToggle("Show Container Name", Settings::esp.containers.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Container Distance", Settings::esp.containers.showDistance);

			ButtonToggle("Container Text Shadowed", Settings::esp.containers.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Container Text Centered", Settings::esp.containers.textCentered);

			if (ImGui::CollapsingHeader("Container Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Container Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.containers.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###ContainerWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.containers.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Junk ESP Settings"))
		{
			ButtonToggle("Junk ESP Enabled", Settings::esp.junk.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.junkEnabledDistance", &Settings::esp.junk.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.junkColor", Settings::esp.junk.color);
			Utils::ValidateRgb(Settings::esp.junk.color);

			ButtonToggle("Draw Enabled Junk", Settings::esp.junk.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.junkEnabledAlpha", &Settings::esp.junk.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			Utils::ValidateFloat(Settings::esp.junk.enabledAlpha, 0.0f, 1.0f);

			ButtonToggle("Draw Disabled Junk", Settings::esp.junk.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.junkDisabledAlpha", &Settings::esp.junk.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Junk", Settings::esp.junk.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Junk", Settings::esp.junk.drawUnnamed);

			ButtonToggle("Show Junk Name", Settings::esp.junk.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Junk Distance", Settings::esp.junk.showDistance);

			ButtonToggle("Junk Text Shadowed", Settings::esp.junk.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Junk Text Centered", Settings::esp.junk.textCentered);

			if (ImGui::CollapsingHeader("Junk Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Junk Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.junk.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###JunkWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.junk.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Plan ESP Settings"))
		{
			ButtonToggle("Plan ESP Enabled", Settings::esp.plans.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.plansEnabledDistance", &Settings::esp.plans.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.plansColor", Settings::esp.plans.color);
			Utils::ValidateRgb(Settings::esp.plans.color);

			ButtonToggle("Draw Enabled Plans", Settings::esp.plans.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.plansEnabledAlpha", &Settings::esp.plans.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Plans", Settings::esp.plans.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.plansDisabledAlpha", &Settings::esp.plans.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Known Plans", Settings::esp.recipes.knownRecipesEnabled);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unknown Plans", Settings::esp.recipes.unknownRecipesEnabled);

			ButtonToggle("Draw Named Plans", Settings::esp.plans.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Plans", Settings::esp.plans.drawUnnamed);

			ButtonToggle("Show Plan Name", Settings::esp.plans.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Plan Distance", Settings::esp.plans.showDistance);

			ButtonToggle("Plan Text Shadowed", Settings::esp.plans.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Plan Text Centered", Settings::esp.plans.textCentered);

			if (ImGui::CollapsingHeader("Plan Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Plan Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.plans.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###PlanWhitelis{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.plans.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Magazine ESP Settings"))
		{
			ButtonToggle("Magazine ESP Enabled", Settings::esp.magazines.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.magazinesEnabledDistance", &Settings::esp.magazines.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.magazinesColor", Settings::esp.magazines.color);
			Utils::ValidateRgb(Settings::esp.magazines.color);

			ButtonToggle("Draw Enabled Magazines", Settings::esp.magazines.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.magazinesEnabledAlpha", &Settings::esp.magazines.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Magazines", Settings::esp.magazines.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.magazinesDisabledAlpha", &Settings::esp.magazines.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Magazines", Settings::esp.magazines.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Magazines", Settings::esp.magazines.drawUnnamed);

			ButtonToggle("Show Magazine Name", Settings::esp.magazines.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Magazine Distance", Settings::esp.magazines.showDistance);

			ButtonToggle("Magazine Text Shadowed", Settings::esp.magazines.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Magazine Text Centered", Settings::esp.magazines.textCentered);

			if (ImGui::CollapsingHeader("Magazine Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Magazine Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.magazines.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###MagazineWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.magazines.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Bobblehead ESP Settings"))
		{
			ButtonToggle("Bobblehead ESP Enabled", Settings::esp.bobbleheads.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.bobbleheadsEnabledDistance", &Settings::esp.bobbleheads.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.bobbleheadsColor", Settings::esp.bobbleheads.color);
			Utils::ValidateRgb(Settings::esp.bobbleheads.color);

			ButtonToggle("Draw Enabled Bobbleheads", Settings::esp.bobbleheads.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.bobbleheadsEnabledAlpha", &Settings::esp.bobbleheads.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Bobbleheads", Settings::esp.bobbleheads.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.bobbleheadsDisabledAlpha", &Settings::esp.bobbleheads.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Bobbleheads", Settings::esp.bobbleheads.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Bobbleheads", Settings::esp.bobbleheads.drawUnnamed);

			ButtonToggle("Show Bobblehead Name", Settings::esp.bobbleheads.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Bobblehead Distance", Settings::esp.bobbleheads.showDistance);

			ButtonToggle("Bobblehead Text Shadowed", Settings::esp.bobbleheads.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Bobblehead Text Centered", Settings::esp.bobbleheads.textCentered);

			if (ImGui::CollapsingHeader("Bobblehead Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Bobblehead Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.bobbleheads.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###BobbleheadWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.bobbleheads.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Item ESP Settings"))
		{
			ButtonToggle("Item ESP Enabled", Settings::esp.items.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.itemsEnabledDistance", &Settings::esp.items.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.itemsColor", Settings::esp.items.color);
			Utils::ValidateRgb(Settings::esp.items.color);

			ButtonToggle("Draw Enabled Items", Settings::esp.items.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.itemsEnabledAlpha", &Settings::esp.items.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Items", Settings::esp.items.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.itemsDisabledAlpha", &Settings::esp.items.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Items", Settings::esp.items.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Items", Settings::esp.items.drawUnnamed);

			ButtonToggle("Show Item Name", Settings::esp.items.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Item Distance", Settings::esp.items.showDistance);

			ButtonToggle("Item Text Shadowed", Settings::esp.items.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Item Text Centered", Settings::esp.items.textCentered);

			if (ImGui::CollapsingHeader("Item Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Item Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.items.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###ItemWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.items.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Flora ESP Settings"))
		{
			ButtonToggle("Flora ESP Enabled", Settings::esp.flora.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###esp.floraEnabledDistance", &Settings::esp.flora.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###esp.floraColor", Settings::esp.flora.color);
			Utils::ValidateRgb(Settings::esp.flora.color);

			ButtonToggle("Draw Enabled Flora", Settings::esp.flora.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.floraEnabledAlpha", &Settings::esp.flora.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Flora", Settings::esp.flora.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###esp.floraDisabledAlpha", &Settings::esp.flora.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Flora", Settings::esp.flora.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Flora", Settings::esp.flora.drawUnnamed);

			LargeButtonToggle("Draw Raw Crimson Flux Yielding Flora", Settings::esp.flux.crimsonFluxEnabled);
			LargeButtonToggle("Draw Raw Cobalt Flux Yielding Flora", Settings::esp.flux.cobaltFluxEnabled);
			LargeButtonToggle("Draw Raw Yellowcake Flux Yielding Flora", Settings::esp.flux.yellowcakeFluxEnabled);
			LargeButtonToggle("Draw Raw Fluorescent Flux Yielding Flora", Settings::esp.flux.fluorescentFluxEnabled);
			LargeButtonToggle("Draw Raw Violet Flux Yielding Flora", Settings::esp.flux.violetFluxEnabled);

			ButtonToggle("Show Flora Name", Settings::esp.flora.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Flora Distance", Settings::esp.flora.showDistance);

			ButtonToggle("Flora Text Shadowed", Settings::esp.flora.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Flora Text Centered", Settings::esp.flora.textCentered);

			if (ImGui::CollapsingHeader("Flora Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Flora Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.flora.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###FloraWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.flora.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Entity ESP Settings"))
		{
			ButtonToggle("Entity ESP Enabled", Settings::esp.entities.enabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###EntitySettingsEnabledDistance", &Settings::esp.entities.enabledDistance, 0, 3000, "Distance: %d");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::ColorEdit3("###EntitySettingsColor", Settings::esp.entities.color);
			Utils::ValidateRgb(Settings::esp.entities.color);

			ButtonToggle("Draw Enabled Entities", Settings::esp.entities.drawEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###EntitySettingsEnabledAlpha", &Settings::esp.entities.enabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Disabled Entities", Settings::esp.entities.drawDisabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###EntitySettingsDisabledAlpha", &Settings::esp.entities.disabledAlpha, 0.0f, 1.0f, "Alpha: %.2f");

			ButtonToggle("Draw Named Entities", Settings::esp.entities.drawNamed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Draw Unnamed Entities", Settings::esp.entities.drawUnnamed);

			ButtonToggle("Show Entity Name", Settings::esp.entities.showName);
			ImGui::SameLine(235.0f);
			ButtonToggle("Show Entity Distance", Settings::esp.entities.showDistance);

			ButtonToggle("Entity Text Shadowed", Settings::esp.entities.textShadowed);
			ImGui::SameLine(235.0f);
			ButtonToggle("Entity Text Centered", Settings::esp.entities.textCentered);

			if (ImGui::CollapsingHeader("Entity Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Entity Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::esp.flora.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###EntityWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::esp.entities.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}
		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuTabInfoBox()
{
	if (ImGui::BeginTabItem("InfoBox###InfoBoxTab"))
	{
		LargeButtonToggle("Draw Automatic Scrap Looter Status", Settings::infobox.drawScrapLooterStatus);
		LargeButtonToggle("Draw Automatic Item Looter Status", Settings::infobox.drawItemLooterStatus);
		LargeButtonToggle("Draw NPC Looter Status", Settings::infobox.drawNpcLooterStatus);
		LargeButtonToggle("Draw Container Looter Status", Settings::infobox.drawContainerLooterStatus);
		LargeButtonToggle("Draw Flora Harvester Status", Settings::infobox.drawHarvesterStatus);
		LargeButtonToggle("Draw Position Status", Settings::infobox.drawPositionSpoofingStatus);
		LargeButtonToggle("Draw Nuke Codes", Settings::infobox.drawNukeCodes);

		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuLooter()
{
	if (ImGui::BeginTabItem("Looter"))
	{
		if (ImGui::CollapsingHeader("Mode"))
		{
			if (ImGui::RadioButton("Disabled", Settings::looter.mode == LooterSettings::Mode::Disabled))
				Settings::looter.mode = LooterSettings::Mode::Disabled;
			if (ImGui::RadioButton("Automatic looting", Settings::looter.mode == LooterSettings::Mode::Auto))
				Settings::looter.mode = LooterSettings::Mode::Auto;
			if (ImGui::RadioButton("Keybind (CTRL + R)", Settings::looter.mode == LooterSettings::Mode::Keybind))
				Settings::looter.mode = LooterSettings::Mode::Keybind;
		}

		if (ImGui::CollapsingHeader("Looters"))
		{
			LargeButtonToggle("Loot NPCs (76m)", Settings::looter.looters.npcs);
			LargeButtonToggle("Loot Items (76m)", Settings::looter.looters.items);
			LargeButtonToggle("Loot Containers (6m)", Settings::looter.looters.containers);
			LargeButtonToggle("Harvest Flora (6m)", Settings::looter.looters.flora);
		}

		if (ImGui::TreeNodeEx("Selection", ImGuiTreeNodeFlags_Framed)) {

			if (ImGui::CollapsingHeader("Weapons"))
			{
				LargeButtonToggle("All##weapons", Settings::looter.selection.weapons.all);
				LargeButtonToggle("1*##weapons", Settings::looter.selection.weapons.oneStar);
				LargeButtonToggle("2*##weapons", Settings::looter.selection.weapons.twoStar);
				LargeButtonToggle("3*##weapons", Settings::looter.selection.weapons.threeStar);
			}

			if (ImGui::CollapsingHeader("Apparel"))
			{
				LargeButtonToggle("All##apparel", Settings::looter.selection.apparel.all);
				LargeButtonToggle("1*##apparel", Settings::looter.selection.apparel.oneStar);
				LargeButtonToggle("2*##apparel", Settings::looter.selection.apparel.twoStar);
				LargeButtonToggle("3*##apparel", Settings::looter.selection.apparel.threeStar);
			}

			if (ImGui::CollapsingHeader("Aid"))
			{
				LargeButtonToggle("All##aid", Settings::looter.selection.aid.all);
				LargeButtonToggle("Bobbleheads##aid", Settings::looter.selection.aid.bobbleheads);
				LargeButtonToggle("Magazines##aid", Settings::looter.selection.aid.magazines);
			}

			if (ImGui::CollapsingHeader("Misc"))
			{
				LargeButtonToggle("All##misc", Settings::looter.selection.misc.all);
			}

			if (ImGui::CollapsingHeader("Holotapes"))
			{
				LargeButtonToggle("All##holo", Settings::looter.selection.holo.all);
			}

			if (ImGui::CollapsingHeader("Notes"))
			{
				LargeButtonToggle("All##notes", Settings::looter.selection.notes.all);
				LargeButtonToggle("Known Plans##notes", Settings::looter.selection.notes.plansKnown);
				LargeButtonToggle("Unknown Plans##notes", Settings::looter.selection.notes.plansUnknown);
				LargeButtonToggle("Treasure Maps##notes", Settings::looter.selection.notes.treasureMaps);
			}

			if (ImGui::CollapsingHeader("Junk"))
			{
				LargeButtonToggle("All##junk", Settings::looter.selection.junk.all);

				ImGui::Columns(2, nullptr, false);
				for (auto& component : Settings::looter.selection.junk.components)
				{
					auto label = fmt::format("{}##junk", JUNK_COMPONENT_NAMES.find(component.first)->second);
					LargeButtonToggle(label.c_str(), component.second);

					ImGui::NextColumn();
				}
				ImGui::Columns();
			}

			if (ImGui::CollapsingHeader("Flora"))
			{
				LargeButtonToggle("All##flora", Settings::looter.selection.flora.all);

				ImGui::Columns(2, nullptr, false);
				for (auto& component : Settings::looter.selection.flora.components)
				{
					auto label = fmt::format("{}##flora", FLORA_COMPONENT_NAMES.find(component.first)->second);
					LargeButtonToggle(label.c_str(), component.second);

					ImGui::NextColumn();
				}
				ImGui::Columns();
			}

			if (ImGui::CollapsingHeader("Mods"))
			{
				LargeButtonToggle("All##mods", Settings::looter.selection.mods.all);
			}

			if (ImGui::CollapsingHeader("Ammo"))
			{
				LargeButtonToggle("All##ammo", Settings::looter.selection.ammo.all);
			}

			if (ImGui::CollapsingHeader("Other"))
			{
				LargeButtonToggle("Caps##other", Settings::looter.selection.other.caps);
			}

			if (ImGui::CollapsingHeader("Whitelist"))
			{
				ImGui::Columns(2, nullptr, false);

				for (auto& item : Settings::looter.selection.whitelist)
				{
					auto toggleLabel = fmt::format("Enabled##whiteList{0:x}Enabled", item.first);
					LargeButtonToggle(toggleLabel.c_str(), item.second);

					ImGui::NextColumn();

					auto inputLabel = fmt::format("##whiteList{0:x}Item", item.first);
					auto key = item.first;
					auto value = item.second;
					if (ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &key, nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						Settings::looter.selection.whitelist.erase(item.first);
						if (key)
							Settings::looter.selection.whitelist.try_emplace(key, value);
					}

					ImGui::NextColumn();
				}
				//this is for inserting new records into the map
				{
					DWORD key = 0;
					auto value = false;

					LargeButtonToggle("Enabled##whiteListNewEnabled", value);

					ImGui::NextColumn();

					if (ImGui::InputScalar("##whiteListNewItem", ImGuiDataType_U32, &key, nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (key)
							Settings::looter.selection.whitelist.try_emplace(key, value);
					}
				}

				ImGui::Columns();
			}

			if (ImGui::CollapsingHeader("Blacklist"))
			{
				ImGui::Columns(2, nullptr, false);

				for (auto& item : Settings::looter.selection.blacklist)
				{
					auto toggleLabel = fmt::format("Enabled##blackList{0:x}Enabled", item.first);
					LargeButtonToggle(toggleLabel.c_str(), item.second);

					ImGui::NextColumn();

					auto inputLabel = fmt::format("##blackList{0:x}Item", item.first);
					auto key = item.first;
					auto value = item.second;
					if (ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &key, nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						Settings::looter.selection.blacklist.erase(item.first);
						if (key)
							Settings::looter.selection.blacklist.try_emplace(key, value);
					}

					ImGui::NextColumn();
				}
				//this is for inserting new records into the map
				{
					DWORD key = 0;
					auto value = false;

					LargeButtonToggle("Enabled##blackListNewEnabled", value);

					ImGui::NextColumn();

					if (ImGui::InputScalar("##blackListNewItem", ImGuiDataType_U32, &key, nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (key)
							Settings::looter.selection.blacklist.try_emplace(key, value);
					}
				}

				ImGui::Columns();
			}

			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuTabCombat()
{
	if (ImGui::BeginTabItem("Combat###CombatTab"))
	{
		if (ImGui::CollapsingHeader("Weapon Editor"))
		{
			ButtonToggle("No Recoil", Settings::weapons.noRecoil);
			ImGui::SameLine(235.0f);
			ButtonToggle("Infinite Ammo", Settings::weapons.infiniteAmmo);

			ButtonToggle("No Spread", Settings::weapons.noSpread);
			ImGui::SameLine(235.0f);
			ButtonToggle("Instant Reload", Settings::weapons.instantReload);

			ButtonToggle("No Sway", Settings::weapons.noSway);
			ImGui::SameLine(235.0f);
			ButtonToggle("Automatic Flag###WeaponAutomatic", Settings::weapons.automaticflag);

			ButtonToggle("Capacity###WeaponCapacityEnabled", Settings::weapons.capacityEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###WeaponCapacity", &Settings::weapons.capacity, 0, 999, "Capacity: %d");

			ButtonToggle("Speed###WeaponSpeedEnabled", Settings::weapons.speedEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###WeaponSpeed", &Settings::weapons.speed, 0.0f, 100.0f, "Speed: %.2f");

			ButtonToggle("Reach###WeaponReachEnabled", Settings::weapons.reachEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###WeaponReach", &Settings::weapons.reach, 0.0f, 999.0f, "Reach: %.2f");
		}

		if (ImGui::CollapsingHeader("Targeting Settings"))
		{
			ButtonToggle("Player Targeting (Keybind: T)", Settings::targetting.lockPlayers);
			ImGui::SameLine(235.0f);
			ButtonToggle("NPC Targeting (Keybind: T)", Settings::targetting.lockNpCs);

			ButtonToggle("Damage Redirection (Players)", Settings::targetting.indirectPlayers);
			ImGui::SameLine(235.0f);
			ButtonToggle("Damage Redirection (NPCs)", Settings::targetting.indirectNpCs);

			ButtonToggle("Send Damage (Players)", Settings::targetting.directPlayers);
			ImGui::SameLine(235.0f);
			ButtonToggle("Send Damage (NPCs)", Settings::targetting.directNpCs);

			SmallButtonToggle("Living###TargetLiving", Settings::targetting.targetLiving);
			ImGui::SameLine(122.0f);
			SmallButtonToggle("Downed###TargetDowned", Settings::targetting.targetDowned);
			ImGui::SameLine(235.0f);
			SmallButtonToggle("Dead###TargetDead", Settings::targetting.targetDead);
			ImGui::SameLine(349.0f);
			SmallButtonToggle("Unknown###TargetUnknown", Settings::targetting.targetUnknown);

			ButtonToggle("Ignore Render Distance###IgnoreRenderDistance", Settings::targetting.ignoreRenderDistance);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderFloat("###TargetLockingFOV", &Settings::targetting.lockingFov, 5.0f, 40.0f, "Targeting FOV: %.2f");

			ButtonToggle("Ignore Essential NPCs###IgnoreEssentialNPCs", Settings::targetting.ignoreEssentialNpCs);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::ColorEdit3("###TargetLockingColor", Settings::targetting.lockingColor);
			Utils::ValidateRgb(Settings::esp.players.unknownColor);

			ButtonToggle("Automatic Retargeting###TargetLockingRetargeting", Settings::targetting.retargeting);

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(224.0f);
				auto sliderText = fmt::format("Cooldown: {0:d} ({1:d} ms)", Settings::targetting.cooldown, Settings::targetting.cooldown * 16);
				ImGui::SliderInt("###TargetLockingCooldown", &Settings::targetting.cooldown, 0, 120, sliderText.c_str());
			}

			{
				ImGui::SetNextItemWidth(224.0f);
				auto sliderText = fmt::format("Send Damage (Min): {0:d} ({1:d} ms)", Settings::targetting.sendDamageMin, Settings::targetting.sendDamageMin * 16);
				if (ImGui::SliderInt("###SendDamageMin", &Settings::targetting.sendDamageMin, 1, 60, sliderText.c_str()))
				{
					if (Settings::targetting.sendDamageMax < Settings::targetting.sendDamageMin)
						Settings::targetting.sendDamageMax = Settings::targetting.sendDamageMin;
				}
			}

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(224.0f);
				auto sliderText = fmt::format("Send Damage (Max): {0:d} ({1:d} ms)", Settings::targetting.sendDamageMax, Settings::targetting.sendDamageMax * 16);
				if (ImGui::SliderInt("###SendDamageMax", &Settings::targetting.sendDamageMax, 1, 60, sliderText.c_str()))
				{
					if (Settings::targetting.sendDamageMax < Settings::targetting.sendDamageMin)
						Settings::targetting.sendDamageMin = Settings::targetting.sendDamageMax;
				}
			}

			{
				std::string favoritedWeaponsPreview = "[?] No Weapon Selected";
				if (Settings::targetting.favoriteIndex < 12)
				{
					favoritedWeaponsPreview = ErectusMemory::GetFavoritedWeaponText(BYTE(Settings::targetting.favoriteIndex));
					if (favoritedWeaponsPreview.empty())
					{
						favoritedWeaponsPreview = fmt::format("[{}] Favorited Item Invalid", ErectusMemory::FavoriteIndex2Slot(BYTE(Settings::targetting.favoriteIndex)));
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
								Settings::targetting.favoriteIndex = item.first - 1;
							else
								Settings::targetting.favoriteIndex = 12;
						}
					}
					Utils::ValidateInt(Settings::targetting.favoriteIndex, 0, 12);

					ImGui::EndCombo();
				}
			}
		}

		if (ImGui::CollapsingHeader("Melee Settings"))
		{
			LargeButtonToggle("Melee Enabled (Keybind: U)", Settings::melee.enabled);

			{
				ImGui::SetNextItemWidth(224.0f);
				auto sliderText = fmt::format("Melee Speed (Min): {0:d} ({1:d} ms)", Settings::melee.speedMin, Settings::melee.speedMin * 16);
				if (ImGui::SliderInt("###MeleeSpeedMin", &Settings::melee.speedMin, 1, 60, sliderText.c_str()))
				{
					if (Settings::melee.speedMax < Settings::melee.speedMin)
						Settings::melee.speedMax = Settings::melee.speedMin;
				}
			}

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(224.0f);
				auto sliderText = fmt::format("Melee Speed (Max): {0:d} ({1:d} ms)", Settings::melee.speedMax, Settings::melee.speedMax * 16);
				if (ImGui::SliderInt("###MeleeSpeedMax", &Settings::melee.speedMax, 1, 60, sliderText.c_str()))
				{
					if (Settings::melee.speedMax < Settings::melee.speedMin)
						Settings::melee.speedMin = Settings::melee.speedMax;
				}
			}
		}

		if (ImGui::CollapsingHeader("One Position Kill"))
		{
			LargeButtonToggle("OPK Players (Keybind: CTRL+B)", Settings::opk.playersEnabled);
			LargeButtonToggle("OPK NPCs (Keybind: CTRL+N)", Settings::opk.npcsEnabled);
		}

		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuTabPlayer()
{
	if (ImGui::BeginTabItem("Player###PlayerTab"))
	{
		if (ImGui::CollapsingHeader("Local Player Settings"))
		{
			LargeButtonToggle("Draw Local Player Data", Settings::infobox.drawLocalPlayerInfo);

			ImGui::SameLine(235.0f);

			LargeButtonToggle("Position Spoofing (Keybind CTRL+L)##LocalPlayerPositionSpoofingEnabled", Settings::localPlayer.positionSpoofingEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerPositionSpoofingHeight", &Settings::localPlayer.positionSpoofingHeight, -524287, 524287, "Spoofed Height: %d");

			ButtonToggle("Noclip (Keybind CTRL+Y)###NoclipEnabled", Settings::localPlayer.noclipEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderFloat("###NoclipSpeed", &Settings::localPlayer.noclipSpeed, 0.0f, 2.0f, "Speed: %.5f");

			ButtonToggle("Client State", Settings::localPlayer.clientState);
			ImGui::SameLine(235.0f);
			ButtonToggle("Automatic Client State", Settings::localPlayer.automaticClientState);

			LargeButtonToggle("Freeze Action Points###LocalPlayerFreezeApEnabled", Settings::localPlayer.freezeApEnabled);

			ButtonToggle("Action Points###LocalPlayerAPEnabled", Settings::localPlayer.actionPointsEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerAP", &Settings::localPlayer.actionPoints, 0, 99999, "Action Points: %d");

			ButtonToggle("Strength###LocalPlayerStrengthEnabled", Settings::localPlayer.strengthEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerStrength", &Settings::localPlayer.strength, 0, 99999, "Strength: %d");

			ButtonToggle("Perception###LocalPlayerPerceptionEnabled", Settings::localPlayer.perceptionEnabled);
			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###LocalPlayerPerception", &Settings::localPlayer.perception, 0, 99999, "Perception: %d");

			ButtonToggle("Endurance###LocalPlayerEnduranceEnabled", Settings::localPlayer.enduranceEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerEndurance", &Settings::localPlayer.endurance, 0, 99999, "Endurance: %d");

			ButtonToggle("Charisma###LocalPlayerCharismaEnabled", Settings::localPlayer.charismaEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerCharisma", &Settings::localPlayer.charisma, 0, 99999, "Charisma: %d");

			ButtonToggle("Intelligence###LocalPlayerIntelligenceEnabled", Settings::localPlayer.intelligenceEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerIntelligence", &Settings::localPlayer.intelligence, 0, 99999, "Intelligence: %d");

			ButtonToggle("Agility###LocalPlayerAgilityEnabled", Settings::localPlayer.agilityEnabled);

			ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);

			ImGui::SliderInt("###LocalPlayerAgility", &Settings::localPlayer.agility, 0, 99999, "Agility: %d");

			ButtonToggle("Luck###LocalPlayerLuckEnabled", Settings::localPlayer.luckEnabled);					ImGui::SameLine(235.0f);
			ImGui::SetNextItemWidth(224.0f);
			ImGui::SliderInt("###LocalPlayerLuck", &Settings::localPlayer.luck, 0, 99999, "Luck: %d");
		}

		if (ImGui::CollapsingHeader("Character Settings"))
		{
			LargeButtonToggle("Character Appearance Editing Enabled###ChargenEditingEnabled", Settings::characterEditor.enabled);
			ImGui::SetNextItemWidth(451.0f);
			ImGui::SliderFloat("###ChargenThin", &Settings::characterEditor.thin, 0.0f, 1.0f, "Character Appearance (Thin): %f");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::SliderFloat("###ChargenMuscular", &Settings::characterEditor.muscular, 0.0f, 1.0f, "Character Appearance (Muscular): %f");

			ImGui::SetNextItemWidth(451.0f);
			ImGui::SliderFloat("###ChargenLarge", &Settings::characterEditor.large, 0.0f, 1.0f, "Character Appearance (Large): %f");
		}
		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuTabUtilities()
{
	if (ImGui::BeginTabItem("Utility###UtilityTab"))
	{
		if (ImGui::CollapsingHeader("Utility"))
		{
			ButtonToggle("ESP Debug Mode", Settings::utilities.debugEsp);

			{
				if (Settings::utilities.ptrFormId)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

					if (ImGui::Button("Get Pointer###GetPointerEnabled", ImVec2(224.0f, 0.0f)))
						getPtrResult = ErectusMemory::GetPtr(Settings::utilities.ptrFormId);

					ImGui::PopStyleColor(3);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::Button("Get Pointer###GetPointerDisabled", ImVec2(224.0f, 0.0f));
					ImGui::PopItemFlag();

					ImGui::PopStyleColor(3);
				}
			}

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(80.0f);
				if (ImGui::InputScalar("###PtrFormIdText", ImGuiDataType_U32, &Settings::utilities.ptrFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal))
					getPtrResult = 0;
			}

			ImGui::SameLine(318.0f);

			{
				ImGui::SetNextItemWidth(141.0f);
				auto inputText = fmt::format("{:16X}", getPtrResult);
				ImGui::InputText("###PtrPointerText", &inputText, ImGuiInputTextFlags_ReadOnly);
			}

			{
				if (Settings::utilities.addressFormId)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

					if (ImGui::Button("Get Address###GetAddressEnabled", ImVec2(224.0f, 0.0f)))
						getAddressResult = ErectusMemory::GetAddress(Settings::utilities.addressFormId);

					ImGui::PopStyleColor(3);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::Button("Get Address###GetAddressDisabled", ImVec2(224.0f, 0.0f));
					ImGui::PopItemFlag();

					ImGui::PopStyleColor(3);
				}
			}

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(80.0f);

				if (ImGui::InputScalar("###AddressFormIdText", ImGuiDataType_U32, &Settings::utilities.addressFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal))
					getAddressResult = 0;
			}

			ImGui::SameLine(318.0f);

			{
				ImGui::SetNextItemWidth(141.0f);

				auto inputText = fmt::format("{:16X}", getAddressResult);
				ImGui::InputText("###AddressPointerText", &inputText, ImGuiInputTextFlags_ReadOnly);
			}
		}

		if (ImGui::CollapsingHeader("Reference Editor"))
		{
			ButtonToggle("Source FormId###SwapperSourceFormIdToggle", swapperSourceToggle);

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(224.0f);
				ImGui::InputScalar("###SwapperSourceFormIdText", ImGuiDataType_U32, &Settings::swapper.sourceFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
			}

			ButtonToggle("Destination FormId###SwapperDestinationFormIdToggle", swapperDestinationToggle);

			ImGui::SameLine(235.0f);

			{
				ImGui::SetNextItemWidth(224.0f);
				ImGui::InputScalar("###SwapperDestinationFormIdText", ImGuiDataType_U32, &Settings::swapper.destinationFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
			}

			if (swapperSourceToggle && Settings::swapper.sourceFormId && swapperDestinationToggle && Settings::swapper.destinationFormId)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

				if (ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceEnabled", ImVec2(451.0f, 0.0f)))
				{
					if (ErectusMemory::ReferenceSwap(Settings::swapper.sourceFormId, Settings::swapper.destinationFormId))
					{
						Settings::swapper.destinationFormId = Settings::swapper.sourceFormId;
						swapperSourceToggle = false;
						swapperDestinationToggle = false;
					}
				}

				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::Button("Edit Reference (Overwrite Destination)###EditReferenceDisabled", ImVec2(451.0f, 0.0f));
				ImGui::PopItemFlag();

				ImGui::PopStyleColor(3);
			}
		}

		if (ImGui::CollapsingHeader("Item Transferring"))
		{
			SmallButtonToggle("Source###TransferSourceFormIdToggle", transferSourceToggle);

			ImGui::SameLine(122.0f);

			{
				ImGui::SetNextItemWidth(110.0f);
				ImGui::InputScalar("###TransferSourceFormIdText", ImGuiDataType_U32, &Settings::customTransferSettings.sourceFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
			}

			ImGui::SameLine(235.0f);

			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

				if (ImGui::Button("Get Player###TransferSourceLocalPlayer", ImVec2(110.0f, 0.0f)))
					Settings::customTransferSettings.sourceFormId = ErectusMemory::GetLocalPlayerFormId();

				ImGui::SameLine(349.0f);

				if (ImGui::Button("Get STASH###TransferSourceSTASH", ImVec2(110.0f, 0.0f)))
					Settings::customTransferSettings.sourceFormId = ErectusMemory::GetStashFormId();

				ImGui::PopStyleColor(3);
			}

			SmallButtonToggle("Destination###TransferDestinationFormIdToggle", transferDestinationToggle);

			ImGui::SameLine(122.0f);

			{
				ImGui::SetNextItemWidth(110.0f);
				ImGui::InputScalar("###TransferDestinationFormIdText", ImGuiDataType_U32, &Settings::customTransferSettings.destinationFormId,
					nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
			}

			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
				ImGui::SameLine(235.0f);
				if (ImGui::Button("Get Player###TransferDestinationLocalPlayer", ImVec2(110.0f, 0.0f)))
					Settings::customTransferSettings.destinationFormId = ErectusMemory::GetLocalPlayerFormId();

				ImGui::SameLine(349.0f);
				if (ImGui::Button("Get STASH###TransferDestinationSTASH", ImVec2(110.0f, 0.0f)))
					Settings::customTransferSettings.destinationFormId = ErectusMemory::GetStashFormId();
				ImGui::PopStyleColor(3);
			}

			auto allowTransfer = false;

			if (transferSourceToggle && Settings::customTransferSettings.sourceFormId && transferDestinationToggle && Settings::customTransferSettings.destinationFormId)
			{
				if (Settings::customTransferSettings.useWhitelist)
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
					ErectusMemory::TransferItems(Settings::customTransferSettings.sourceFormId, Settings::customTransferSettings.destinationFormId);

				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::Button("Transfer Items###TransferItemsDisabled", ImVec2(451.0f, 0.0f));
				ImGui::PopItemFlag();

				ImGui::PopStyleColor(3);
			}

			LargeButtonToggle("Use Item Transfer Whitelist", Settings::customTransferSettings.useWhitelist);
			LargeButtonToggle("Use Item Transfer Blacklist", Settings::customTransferSettings.useBlacklist);

			if (ImGui::CollapsingHeader("Item Transfer Whitelist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Transfer Whitelist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::customTransferSettings.whitelisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###ItemTransferWhitelist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::customTransferSettings.whitelist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}

			if (ImGui::CollapsingHeader("Item Transfer Blacklist Settings"))
			{
				for (auto i = 0; i < 32; i++)
				{
					auto toggleLabel = fmt::format("Transfer Blacklist Slot: {0:d}", i);
					ButtonToggle(toggleLabel.c_str(), Settings::customTransferSettings.blacklisted[i]);

					ImGui::SameLine(235.0f);
					ImGui::SetNextItemWidth(224.0f);

					auto inputLabel = fmt::format("###ItemTransferBlacklist{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::customTransferSettings.blacklist[i],
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}
			}
		}

		if (ImGui::CollapsingHeader("Nuke Codes"))
		{
			ButtonToggle("Automatic Nuke Codes", Settings::customNukeCodeSettings.automaticNukeCodes);

			ImGui::SameLine(235.0f);

			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

				if (ImGui::Button("Get Nuke Codes", ImVec2(224.0f, 0.0f)))
					ErectusMemory::UpdateNukeCodes();

				ImGui::PopStyleColor(3);
			}

			auto text = format("{} - Alpha", fmt::join(ErectusMemory::alphaCode, " "));
			ImGui::Text(text.c_str());

			text = format("{} - Bravo", fmt::join(ErectusMemory::bravoCode, " "));
			ImGui::Text(text.c_str());

			text = format("{} - Charlie", fmt::join(ErectusMemory::charlieCode, " "));
			ImGui::Text(text.c_str());
		}

		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenuTabTeleporter()
{
	if (ImGui::BeginTabItem("Teleporter###TeleporterTab"))
	{
		for (auto i = 0; i < 16; i++)
		{
			auto teleportHeaderText = fmt::format("Teleport Slot: {0:d}", i);
			if (ImGui::CollapsingHeader(teleportHeaderText.c_str()))
			{
				{
					ImGui::SetNextItemWidth(110.0f);
					auto inputLabel = fmt::format("###TeleportDestinationX{:d}", i);
					ImGui::InputFloat(inputLabel.c_str(), &Settings::teleporter.entries[i].destination[0]);
				}

				ImGui::SameLine(122.0f);

				{
					ImGui::SetNextItemWidth(110.0f);
					auto inputLabel = fmt::format("###TeleportDestinationY{:d}", i);
					ImGui::InputFloat(inputLabel.c_str(), &Settings::teleporter.entries[i].destination[1]);
				}

				ImGui::SameLine(235.0f);

				{
					ImGui::SetNextItemWidth(110.0f);
					auto inputLabel = fmt::format("###TeleportDestinationZ{:d}", i);
					ImGui::InputFloat(inputLabel.c_str(), &Settings::teleporter.entries[i].destination[2]);
				}

				ImGui::SameLine(349.0f);

				{
					ImGui::SetNextItemWidth(110.0f);
					auto inputLabel = fmt::format("###TeleportDestinationW{:d}", i);
					ImGui::InputFloat(inputLabel.c_str(), &Settings::teleporter.entries[i].destination[3]);
				}

				{
					ImGui::SetNextItemWidth(110.0f);
					auto inputLabel = fmt::format("###TeleportCellFormId{:d}", i);
					ImGui::InputScalar(inputLabel.c_str(), ImGuiDataType_U32, &Settings::teleporter.entries[i].cellFormId,
						nullptr, nullptr, "%08lX", ImGuiInputTextFlags_CharsHexadecimal);
				}

				ImGui::SameLine(122.0f);

				{
					auto buttonLabel = fmt::format("Set Position###TeleportDestination{:d}", i);
					if (!Settings::teleporter.entries[i].disableSaving)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.5f));

						if (ImGui::Button(buttonLabel.c_str(), ImVec2(110.0f, 0.0f)))
							ErectusMemory::GetTeleportPosition(i);

						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));

						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::Button(buttonLabel.c_str(), ImVec2(110.0f, 0.0f));
						ImGui::PopItemFlag();

						ImGui::PopStyleColor(3);
					}
				}

				ImGui::SameLine(235.0f);

				{
					auto buttonLabel = fmt::format("Lock###DisableSaving{:d}", i);
					SmallButtonToggle(buttonLabel.c_str(), Settings::teleporter.entries[i].disableSaving);
				}

				ImGui::SameLine(349.0f);

				if (Settings::teleporter.entries[i].cellFormId)
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
}

void Gui::OverlayMenuTabBitMsgWriter()
{
	if (ImGui::BeginTabItem("BitMsgWriter###BitMsgWriterTab"))
	{
		LargeButtonToggle("Message Sender Enabled", Settings::msgWriter.enabled);

		ImGui::EndTabItem();
	}
}

void Gui::OverlayMenu()
{
	if (!App::overlayMenuActive)
		return;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(App::windowSize[0]), static_cast<float>(App::windowSize[1])));
	ImGui::SetNextWindowCollapsed(false);

	if (ImGui::Begin("Erectus - Overlay Menu", nullptr,
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Exit"))
				App::CloseWnd();

			if (ImGui::MenuItem("Process Menu"))
				ErectusProcess::SetProcessMenu();

			if (ImGui::MenuItem("Overlay"))
			{
				if (!App::SetOverlayPosition(false, true))
					ErectusProcess::SetProcessMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("###OverlayMenuTabBar", ImGuiTabBarFlags_None))
		{
			OverlayMenuTabEsp();
			OverlayMenuTabCombat();
			OverlayMenuTabPlayer();
			OverlayMenuTabInfoBox();
			OverlayMenuTabUtilities();
			OverlayMenuTabTeleporter();
			OverlayMenuTabBitMsgWriter();

			OverlayMenuLooter();

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

bool Gui::Init()
{
	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(App::appHwnd))
		return false;

	if (!ImGui_ImplDX9_Init(Renderer::d3D9Device))
		return false;

	return true;
}

void Gui::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
