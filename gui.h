#pragma once
#include "ErectusMemory.h"
#include "settings.h"
#include "dependencies/imgui/imgui.h"
#include "game/Datatypes/Camera.h"

class Gui final {
public:
	static void Render();

private:
	enum class Menu
	{
		ProcessMenu,
		SettingsMenu,
	} inline static mode = Menu::ProcessMenu;

	static void ButtonToggle(const char* label, bool& state);
	static void LargeButtonToggle(const char* label, bool& state);
	static void SmallButtonToggle(const char* label, bool& state);

	static void EspSettings(EspSettings::Actors& actorEsp);
	static void EspSettings(EspSettings::Items& itemEsp);

	static void OverlayMenuTabEsp();
	static void OverlayMenuTabLooter();
	static void OverlayMenuTabCombat();
	static void OverlayMenuTabPlayer();
	static void OverlayMenuTabUtilities();
	static void OverlayMenuTabTeleporter();
	static void OverlayMenuTabBitMsgWriter();

	static void MenuBar();
	static void Menu();

	static void RenderOverlay();
	static void RenderInfoBox();

	static void RenderEntities(const Camera& cameraData);
	static void RenderPlayers(const Camera& cameraData);

	static void RenderActors(const CustomEntry& entry, const Camera& camera, const EspSettings::Actors& settings);
	static void RenderItems(const CustomEntry& entry, const Camera& camera, const EspSettings::Items& settings);

	static void RenderText(const char* text, const Vector2& position, ImU32 color);

	static void ProcessMenu();
	static void SettingsMenu();

	inline static bool swapperSourceToggle = false;
	inline static bool swapperDestinationToggle = false;
	inline static bool transferSourceToggle = false;
	inline static bool transferDestinationToggle = false;

	inline static std::uintptr_t getPtrResult = 0;
	inline static std::uintptr_t getAddressResult = 0;

	virtual void Dummy() = 0;
};
