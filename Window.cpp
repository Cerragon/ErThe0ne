#include "Window.hpp"
#include "app.h"
#include "dwmapi.h"
#include "resource.h"
#pragma comment(lib, "dwmapi")

// ReSharper disable once CppInconsistentNaming
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace
{
	const Window* gWindow;
}

LRESULT CALLBACK mainWndProc(const HWND hWnd, const unsigned int msg, const WPARAM wParam, const LPARAM lParam)
{
	// forward messages from a global window procedure to the member window procedure until a valid window handle is available 
	// this is needed because we can't assign a member function to WINDCLASS:lpfnWndProc
	return gWindow->MsgProc(hWnd, msg, wParam, lParam);
}

Window::Window(App* const appInstance, const LPCSTR windowTitle) : app(appInstance)
{
	gWindow = this;
	Init(windowTitle);
}

Window::~Window()
{
	gWindow = nullptr;
}

LRESULT __stdcall Window::MsgProc(const HWND hWnd, const unsigned int msg, const WPARAM wParam, const LPARAM lParam) const
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return 1;

	switch (msg)
	{
	case WM_SIZE:
		app->OnWindowChanged();
		break;
	case WM_DESTROY:
		app->Shutdown();
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::Init(const LPCSTR windowTitle)
{
	const auto& appInstance = app->GetAppInstance();

	WNDCLASSEX wndClass{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_VREDRAW | CS_HREDRAW,
		.lpfnWndProc = mainWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = appInstance,
		.hIcon = LoadIcon(appInstance, MAKEINTRESOURCE(IDI_ICON1)),
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = windowTitle,
		.hIconSm = nullptr
	};

	if (!RegisterClassEx(&wndClass))
		throw std::exception("Failed to register window class");

	mainWindow = CreateWindowEx(0, wndClass.lpszClassName, windowTitle, 0, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, nullptr, nullptr, appInstance, nullptr);
	if (!mainWindow)
		throw std::exception("Failed to create app window");

	SetStyle(Styles::Standalone);
}

void Window::SetPosition(const LONG x, const LONG y) const
{
	SetWindowPos(mainWindow, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

std::pair<LONG, LONG> Window::GetSize() const
{
	RECT  rect = {};
	GetClientRect(mainWindow, &rect);

	return { rect.right - rect.left, rect.bottom - rect.top };
}

void Window::SetSize(const LONG x, const LONG y) const
{
	RECT  rect = {};
	GetClientRect(mainWindow, &rect);

	if (x == rect.right - rect.left && y == rect.bottom - rect.top)
		return;

	rect.right = rect.left + x;
	rect.bottom = rect.top + y;

	AdjustWindowRect(&rect, GetWindowLong(mainWindow, GWL_STYLE), FALSE);
	SetWindowPos(mainWindow, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::SetStyle(const Styles newStyle)
{
	if (newStyle == currentStyle)
		return;

	currentStyle = newStyle;

	SetWindowLong(mainWindow, GWL_STYLE, styles.at(currentStyle).style);
	SetWindowLong(mainWindow, GWL_EXSTYLE, styles.at(currentStyle).styleEx);

	MARGINS margins{ -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(mainWindow, &margins);

	SetWindowPos(mainWindow, styles.at(currentStyle).styleEx & WS_EX_TOPMOST ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
}
