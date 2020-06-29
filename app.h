#pragma once
#include <Windows.h>

class App final {
public:
	static int CreateWnd(HINSTANCE hInstance);
	static void CloseWnd();

	static LRESULT CALLBACK WndCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static bool SetOverlayPosition(bool topmost, bool layered);
	static void SetOverlayMenu();

	static void RegisterHotkeys();

	inline static unsigned int windowSize[2] = { 0, 0 };
	inline static int windowPosition[2] = { 0, 0 };
	inline static bool overlayMenuActive = false;
	inline static bool overlayActive = false;
	inline static bool overlayForeground = false;

	inline static HWND appHwnd = nullptr;

private:
	static void OnHotkey(WPARAM hotkeyId);
	static void ToggleOverlay();
	static void Render();

	static void RenderProcessMenu();
	static void RenderOverlayMenu();
	static void RenderOverlay();

	inline static HINSTANCE mHInstance = nullptr;
	inline static int windowTopmostCounter = 0;

	virtual void __dummy() = 0;
};
