#pragma once
#include <array>
#include <Windows.h>

class Gui final {
public:
	static void ProcessMenu();
	static void OverlayMenu();

	static bool ImGuiInitialize();
	static void ImGuiCleanup();

	inline static std::array<int, 8> alphaCode = { };
	inline static std::array<int, 8> bravoCode = { };
	inline static std::array<int, 8> charlieCode = { };
private:
	static void ButtonToggle(const char* label, bool* state);
	static void LargeButtonToggle(const char* label, bool* state);
	static void SmallButtonToggle(const char* label, bool* state);

	static void OverlayMenuTabEsp();
	static void OverlayMenuTabLoot();
	static void OverlayMenuTabCombat();
	static void OverlayMenuTabPlayer();
	static void OverlayMenuTabUtilities();
	static void OverlayMenuTabTeleporter();
	static void OverlayMenuTabBitMsgWriter();

	inline static bool swapperSourceToggle = false;
	inline static bool swapperDestinationToggle = false;
	inline static bool transferSourceToggle = false;
	inline static bool transferDestinationToggle = false;

	inline static DWORD64 getPtrResult = 0;
	inline static DWORD64 getAddressResult = 0;

	virtual void __dummy() = 0;
};
