#pragma once
#include <map>
#include <Windows.h>

class App;

class Window
{
public:
	enum class Styles
	{
		Unknown,
		Standalone,
		Attached,
		Overlay
	};

	Window(App* appInstance, LPCSTR windowTitle);
	~Window();
	LRESULT __stdcall MsgProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam) const;

	[[nodiscard]] HWND GetHwnd() const { return mainWindow; }

	void SetPosition(LONG x, LONG y) const;
	[[nodiscard]] std::pair<LONG, LONG> GetSize() const;
	void SetSize(LONG x, LONG y) const;
	void SetStyle(Styles newStyle);

private:
	void Init(LPCSTR windowTitle);

	struct WndStyles
	{
		DWORD style;
		DWORD styleEx;
	};
	const std::map<Styles, WndStyles> styles = {
		{ Styles::Standalone, { WS_SYSMENU | WS_CAPTION, 0 } },
		{ Styles::Attached, { WS_POPUP,  WS_EX_TOPMOST } },
		{ Styles::Overlay, { WS_POPUP,  WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT } } //layered+transparent = clickthrough
	};
	Styles currentStyle = Styles::Unknown;
	HWND mainWindow = nullptr;
	App* const app;
};
