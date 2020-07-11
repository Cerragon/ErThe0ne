#include "app.h"
#include "gui.h"
#include "renderer.h"
#include "common.h"
#include "resource.h"
#include "settings.h"
#include "threads.h"

#include <thread>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


#include "ErectusMemory.h"
#include "ErectusProcess.h"


// ReSharper disable once CppInconsistentNaming
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// ReSharper disable once CppParameterMayBeConst
// ReSharper disable once CppParameterMayBeConst
LRESULT CALLBACK App::WndCallback(HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return 1;

	switch (uMsg)
	{
	case WM_HOTKEY:
		OnHotkey(wParam);
	case WM_KEYDOWN:
		return 0;
	case WM_PAINT:
		Render();
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void App::OnHotkey(const WPARAM hotkeyId)
{
	switch (hotkeyId)
	{
	case (static_cast<int>(HotKeys::ContainerLooterToggle)):
		Settings::containerLooter.enabled = !Settings::containerLooter.enabled;
		break;
	case (static_cast<int>(HotKeys::NpcLooterToggle)):
		Settings::npcLooter.enabled = !Settings::npcLooter.enabled;
		break;
	case (static_cast<int>(HotKeys::HarvesterToggle)):
		Settings::harvester.enabled = !Settings::harvester.enabled;
		break;
	case (static_cast<int>(HotKeys::PositionSpoofingToggle)):
		if (Settings::customLocalPlayerSettings.positionSpoofingEnabled)
			Threads::positionSpoofingToggle = !Threads::positionSpoofingToggle;
		break;
	case (static_cast<int>(HotKeys::NoclipToggle)):
		if (Settings::customLocalPlayerSettings.noclipEnabled)
			Threads::noclipToggle = !Threads::noclipToggle;
		break;
	case (static_cast<int>(HotKeys::OpkPlayersToggle)):
		if (Settings::opk.playersEnabled)
			Threads::opkPlayersToggle = !Threads::opkPlayersToggle;
		break;
	case (static_cast<int>(HotKeys::OpkNpcsToggle)):
		if (Settings::opk.npcsEnabled)
			Threads::opkNpcsToggle = !Threads::opkNpcsToggle;
		break;
	case (static_cast<int>(HotKeys::LootItems)):
		if (Settings::itemLooter.keybindEnabled)
			ErectusMemory::RequestLootItems();
		break;
	case (static_cast<int>(HotKeys::LootScrap)):
		if (Settings::scrapLooter.keybindEnabled)
			ErectusMemory::RequestLootScrap();
		break;
	case (static_cast<int>(HotKeys::ToggleOverlay)):
		ToggleOverlay();
		break;
	default:
		break;
	}
}

void App::RegisterHotkeys()
{
	for (auto item : HOTKEYS)
	{
		::RegisterHotKey(appHwnd, static_cast<int>(item.first), item.second.modifiers, item.second.vk);
	}
}

void App::ToggleOverlay()
{
	if (!ErectusProcess::processSelected)
		return;

	if (overlayMenuActive)
		SetOverlayPosition(false, true);
	else
		SetOverlayMenu();
}
void App::Render() {
	switch (Renderer::BeginScene()) {
	case 0: //OK
		break;
	case 1: //TRY LATER
		return;
	case 99: //FATAL ERROR
	default:
		CloseWnd();
	}

	Gui::Render();

	Renderer::EndScene();

	if (ErectusProcess::processSelected)
	{
		ErectusProcess::processValidCounter++;
		if (ErectusProcess::processValidCounter > 20)
		{
			ErectusProcess::processValidCounter = 0;
			if (WaitForSingleObject(ErectusProcess::handle, 0) != WAIT_TIMEOUT)
			{
				ErectusProcess::ResetProcessData();
			}

			if (overlayActive)
			{
				if (ErectusProcess::hWnd == GetForegroundWindow())
				{
					overlayForeground = true;
					if (!SetOverlayPosition(true, true))
						ErectusProcess::SetProcessMenu();
				}
				else
				{
					overlayForeground = false;
					if (!SetOverlayPosition(false, false))
						ErectusProcess::SetProcessMenu();
				}
			}
		}

		if (!Threads::threadCreationState)
			Threads::threadCreationState = Threads::CreateProcessThreads();
	}

	if (Threads::threadDestructionQueued)
	{
		if (!Threads::ThreadDestruction())
		{
			Threads::threadDestructionCounter++;
			if (Threads::threadDestructionCounter > 900)
			{
				Threads::threadDestructionCounter = 0;
				CloseWnd();
			}
		}
	}

	//ghetto run once per frame
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

void App::SetOverlayMenu()
{
	if (windowSize[0] != 480 || windowSize[1] != 480)
	{
		windowSize[0] = 480;
		windowSize[1] = 720;

		Renderer::deviceResetQueued = true;
		SetWindowPos(appHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
	}

	int bufferPosition[2];
	bufferPosition[0] = GetSystemMetrics(SM_CXSCREEN) / 2 - windowSize[0] / 2;
	bufferPosition[1] = GetSystemMetrics(SM_CYSCREEN) / 2 - windowSize[1] / 2;

	if (windowPosition[0] != bufferPosition[0] || windowPosition[1] != bufferPosition[1])
	{
		windowPosition[0] = bufferPosition[0];
		windowPosition[1] = bufferPosition[1];

		MoveWindow(appHwnd, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], FALSE);
		if (!Renderer::deviceResetQueued)
			SetWindowPos(appHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
	}

	auto style = GetWindowLongPtr(appHwnd, GWL_EXSTYLE);

	if (style & WS_EX_LAYERED)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLongPtr(appHwnd, GWL_EXSTYLE, style);
	}

	if (style & WS_EX_TOPMOST)
		SetWindowPos(appHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);

	ErectusProcess::processMenuActive = false;
	overlayMenuActive = true;
	overlayActive = false;
}

int App::CreateWnd(const HINSTANCE hInstance)
{
	mHInstance = hInstance;

	WNDCLASSEX wndClass{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_VREDRAW | CS_HREDRAW,
		.lpfnWndProc = WndCallback,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = mHInstance,
		.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		.hCursor = nullptr,
		.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00)),
		.lpszMenuName = nullptr,
		.lpszClassName = OVERLAY_WINDOW_CLASS,
		.hIconSm = nullptr
	};

	if (!RegisterClassEx(&wndClass))
	{
		return 1;
	}

	windowSize[0] = 384;
	windowSize[1] = 224;
	windowPosition[0] = GetSystemMetrics(SM_CXSCREEN) / 2 - windowSize[0] / 2;
	windowPosition[1] = GetSystemMetrics(SM_CYSCREEN) / 2 - windowSize[1] / 2;
	appHwnd = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED, wndClass.lpszClassName, OVERLAY_WINDOW_NAME, WS_POPUP, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], nullptr, nullptr, wndClass.hInstance, nullptr);

	if (appHwnd == nullptr)
	{
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		return 2;
	}

	MARGINS overlayMargins = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(appHwnd, &overlayMargins);
	SetLayeredWindowAttributes(appHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA);
	SetWindowLongPtr(appHwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT);

	return 0;
}

void App::CloseWnd() {
	if (appHwnd != nullptr)
	{
		SendMessage(appHwnd, WM_CLOSE, 0, 0);
	}
	UnregisterClass(OVERLAY_WINDOW_CLASS, mHInstance);
}

bool App::SetOverlayPosition(const bool topmost, const bool layered)
{
	RECT windowRect;
	RECT clientRect;

	if (!ErectusProcess::HwndValid(ErectusProcess::pid) || !GetWindowRect(ErectusProcess::hWnd, &windowRect) || !GetClientRect(ErectusProcess::hWnd, &clientRect))
	{
		overlayActive = false;
		return false;
	}

	unsigned int size[2];
	size[0] = clientRect.right;
	size[1] = clientRect.bottom;

	int position[2];
	position[0] = windowRect.left - (clientRect.right + windowRect.left - windowRect.right) / 2;
	position[1] = windowRect.top - (clientRect.bottom + windowRect.top - windowRect.bottom) / 2;

	if (GetWindowLongPtr(ErectusProcess::hWnd, GWL_STYLE) & WS_BORDER)
	{
		auto buffer = GetSystemMetrics(SM_CYCAPTION) / 2;
		buffer += buffer & 1;
		position[1] += buffer;
	}

	if (GetMenu(ErectusProcess::hWnd) != nullptr)
	{
		auto buffer = GetSystemMetrics(SM_CYMENU) / 2;
		buffer += buffer & 1;
		position[1] += buffer;
	}

	if (position[0] != windowPosition[0] || position[1] != windowPosition[1])
	{
		windowPosition[0] = position[0];
		windowPosition[1] = position[1];
		MoveWindow(appHwnd, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
	}

	if (size[0] != windowSize[0] || size[1] != windowSize[1])
	{
		windowSize[0] = size[0];
		windowSize[1] = size[1];
		Renderer::deviceResetQueued = true;
	}

	if (topmost || layered)
	{
		auto style = GetWindowLongPtr(appHwnd, GWL_EXSTYLE);

		if (topmost && !(style & WS_EX_TOPMOST))
		{
			SetWindowPos(appHwnd, HWND_TOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
			windowTopmostCounter++;
			if (windowTopmostCounter > 3)
			{
				windowTopmostCounter = 0;
				ErectusProcess::SetProcessError(0, "Process State: Overlay not topmost");
				overlayActive = false;
				return false;
			}
		}
		else if (!topmost && style & WS_EX_TOPMOST)
			SetWindowPos(appHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
		else
			windowTopmostCounter = 0;

		if (layered && !(style & WS_EX_LAYERED))
		{
			style |= WS_EX_LAYERED;
			SetWindowLongPtr(appHwnd, GWL_EXSTYLE, style);
			SetLayeredWindowAttributes(appHwnd, RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);
		}
		else if (!layered && style & WS_EX_LAYERED)
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(appHwnd, GWL_EXSTYLE, style);
		}
	}

	ErectusProcess::processMenuActive = false;
	overlayMenuActive = false;
	overlayActive = true;
	return true;
}
