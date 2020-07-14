#pragma once
#include <Windows.h>
#include "ErectusMemory.h"
#include "settings.h"

class Gui final {
public:
	static void Render();

	static bool Init();
	static void Shutdown();

private:
	static void ButtonToggle(const char* label, bool& state);
	static void LargeButtonToggle(const char* label, bool& state);
	static void SmallButtonToggle(const char* label, bool& state);

	static void OverlayMenuTabEsp();
	static void OverlayMenuTabInfoBox();
	static void OverlayMenuTabLoot();
	static void OverlayMenuTabCombat();
	static void OverlayMenuTabPlayer();
	static void OverlayMenuTabUtilities();
	static void OverlayMenuTabTeleporter();
	static void OverlayMenuTabBitMsgWriter();
	static void OverlayMenuLooter();
	
	static void RenderOverlay();
	static void RenderInfoBox();

	static void RenderEntities();
	static void RenderPlayers();

	static void RenderActors(const CustomEntry& entry, const OverlaySettingsA& settings);
	static void RenderItems(const CustomEntry& entry, const OverlaySettingsB& settings);

	static void ProcessMenu();
	static void OverlayMenu();

	inline static bool swapperSourceToggle = false;
	inline static bool swapperDestinationToggle = false;
	inline static bool transferSourceToggle = false;
	inline static bool transferDestinationToggle = false;

	inline static DWORD64 getPtrResult = 0;
	inline static DWORD64 getAddressResult = 0;
	virtual void __dummy() = 0;
};
