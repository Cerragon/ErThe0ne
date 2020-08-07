#pragma once
#include <Windows.h>
#include "ErectusMemory.h"
#include "settings.h"

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
	static void OverlayMenuTabInfoBox();
	static void OverlayMenuTabCombat();
	static void OverlayMenuTabPlayer();
	static void OverlayMenuTabUtilities();
	static void OverlayMenuTabTeleporter();
	static void OverlayMenuTabBitMsgWriter();
	static void MenuBar();
	static void Menu();
	static void OverlayMenuLooter();

	static void RenderOverlay();
	static void RenderInfoBox();

	static void RenderEntities();
	static void RenderPlayers();

	static void RenderActors(const CustomEntry& entry, const EspSettings::Actors& settings);
	static void RenderItems(const CustomEntry& entry, const EspSettings::Items& settings);

	static void RenderText(const char* text, float* position, const float* color, float alpha);

	static void ProcessMenu();
	static void SettingsMenu();

	inline static bool swapperSourceToggle = false;
	inline static bool swapperDestinationToggle = false;
	inline static bool transferSourceToggle = false;
	inline static bool transferDestinationToggle = false;

	inline static DWORD64 getPtrResult = 0;
	inline static DWORD64 getAddressResult = 0;

	virtual void Dummy() = 0;
};
