#include "ErectusInclude.h"
#include "resource.h"

// ReSharper disable once CppInconsistentNaming
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// ReSharper disable once CppParameterMayBeConst
// ReSharper disable once CppParameterMayBeConst
LRESULT CALLBACK ErectusMain::WndCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return 1;
	}

	switch (uMsg)
	{
	case WM_KEYDOWN:
		KeybindHandler(wParam, lParam);
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

void ErectusMain::Render() {
	switch (Renderer::BeginScene()) {
	case 0: //OK
		break;
	case 1: //TRY LATER
		return;
	case 99: //FATAL ERROR
	default:
		CloseWnd();
	}

	//render
	RenderProcessMenu();
	RenderOverlayMenu();
	RenderOverlay();

	Renderer::EndScene();

	if (ErectusProcess::processSelected)
	{
		ErectusProcess::processValidCounter++;
		if (ErectusProcess::processValidCounter > 20)
		{
			ErectusProcess::processValidCounter = 0;
			if (WaitForSingleObject(ErectusProcess::handle, 0) != WAIT_TIMEOUT)
			{
				ErectusProcess::ResetProcessData(true, 1);
			}

			if (overlayActive)
			{
				if (ErectusProcess::hWnd == GetForegroundWindow())
				{
					overlayForeground = true;
					if (!SetOverlayPosition(true, true))
					{
						ErectusProcess::SetProcessMenu();
					}
				}
				else
				{
					overlayForeground = false;
					if (!SetOverlayPosition(false, false))
					{
						ErectusProcess::SetProcessMenu();
					}
				}
			}
		}

		if (!ErectusThread::threadCreationState)
		{
			ErectusThread::threadCreationState = ErectusThread::CreateProcessThreads();
		}

		if (Utils::DoubleKeyPress(VK_CONTROL, VK_RETURN, &ErectusMain::overlayMenuPress))
		{
			if (overlayMenuActive)
			{
				SetOverlayPosition(false, true);
			}
			else
			{
				SetOverlayMenu();
			}
		}
	}

	if (ErectusThread::threadDestructionQueued)
	{
		if (!ErectusThread::ThreadDestruction())
		{
			ErectusThread::threadDestructionCounter++;
			if (ErectusThread::threadDestructionCounter > 900)
			{
				ErectusThread::threadDestructionCounter = 0;
				CloseWnd();
			}
		}
	}

	auto npcKeyPress = false;
	auto containerKeyPress = false;
	auto floraKeyPress = false;
	if (Utils::DoubleKeyPress(VK_CONTROL, VK_OEM_COMMA, &npcKeyPress))
	{
		if (ErectusIni::npcLooterSettings.entityLooterEnabled)
		{
			ErectusIni::npcLooterSettings.entityLooterEnabled = false;
		}
		else
		{
			ErectusIni::npcLooterSettings.entityLooterEnabled = true;
		}
	}

	if (Utils::DoubleKeyPress(VK_CONTROL, VK_OEM_PERIOD, &containerKeyPress))
	{
		if (ErectusIni::containerLooterSettings.entityLooterEnabled)
		{
			ErectusIni::containerLooterSettings.entityLooterEnabled = false;
		}
		else
		{
			ErectusIni::containerLooterSettings.entityLooterEnabled = true;
		}
	}

	if (Utils::DoubleKeyPress(VK_CONTROL, 'P', &floraKeyPress))
	{
		if (ErectusIni::customHarvesterSettings.harvesterEnabled)
		{
			ErectusIni::customHarvesterSettings.harvesterEnabled = false;
		}
		else
		{
			ErectusIni::customHarvesterSettings.harvesterEnabled = true;
		}
	}

	//ghetto run once per frame
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

void ErectusMain::RenderProcessMenu() {
	if (!ErectusProcess::processMenuActive)
		return;

	ErectusImGui::ProcessMenu();
}

void ErectusMain::RenderOverlayMenu() {
	if (!ErectusMain::overlayMenuActive)
		return;

	ErectusImGui::OverlayMenu();
}

void ErectusMain::RenderOverlay()
{
	if (!overlayActive || !overlayForeground)
		return;

	Renderer::d3DxSprite->Begin(D3DXSPRITE_ALPHABLEND);

	ErectusMemory::targetLockingValid = false;
	ErectusMemory::targetLockingClosestDegrees = ErectusIni::customTargetSettings.lockingFov;
	ErectusMemory::targetLockingClosestPtr = 0;

	ErectusMemory::RenderCustomEntityList();
	ErectusMemory::RenderCustomNpcList();
	ErectusMemory::RenderCustomPlayerList();

	if (!ErectusMemory::targetLockingValid)
	{
		if (ErectusMemory::targetLockingPtr)
		{

			ErectusMemory::targetLockingCooldown = ErectusIni::customTargetSettings.retargeting ? ErectusIni::customTargetSettings.cooldown : -1;
			ErectusMemory::targetLockingPtr = 0;
		}
		else if (ErectusMemory::targetLockingClosestDegrees < ErectusIni::customTargetSettings.lockingFov)
		{
			ErectusMemory::targetLockingCooldown = 0;
			ErectusMemory::targetLockingPtr = ErectusMemory::targetLockingClosestPtr;
		}
	}

	ErectusMemory::RenderData();

	Renderer::d3DxSprite->End();
}

void ErectusMain::SetOverlayMenu()
{
	if (windowSize[0] != 480 || windowSize[1] != 480)
	{
		windowSize[0] = 480;
		windowSize[1] = 720;

		if (windowHwnd != nullptr)
		{
			Renderer::deviceResetQueued = true;
			SetWindowPos(windowHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
		}
	}

	int bufferPosition[2];
	bufferPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - (windowSize[0] / 2);
	bufferPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - (windowSize[1] / 2);

	if (windowPosition[0] != bufferPosition[0] || windowPosition[1] != bufferPosition[1])
	{
		windowPosition[0] = bufferPosition[0];
		windowPosition[1] = bufferPosition[1];

		if (windowHwnd != nullptr)
		{
			MoveWindow(windowHwnd, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], FALSE);
			if (!Renderer::deviceResetQueued)
			{
				SetWindowPos(windowHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
			}
		}
	}

	if (windowHwnd != nullptr)
	{
		auto style = GetWindowLongPtr(windowHwnd, GWL_EXSTYLE);

		if (style & WS_EX_LAYERED)
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(windowHwnd, GWL_EXSTYLE, style);
		}

		if (style & WS_EX_TOPMOST)
		{
			SetWindowPos(windowHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
		}
	}

	ErectusProcess::processMenuActive = false;
	overlayMenuActive = true;
	overlayActive = false;
}

int ErectusMain::CreateWnd(const HINSTANCE hInstance)
{
	mHInstance = hInstance;
	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	wndClass.lpfnWndProc = WndCallback;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = mHInstance;
	wndClass.hIcon = LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = OVERLAY_WINDOW_CLASS;
	wndClass.hIconSm = nullptr;

	if (!RegisterClassEx(&wndClass))
	{
		return 1;
	}

	windowSize[0] = 384;
	windowSize[1] = 224;
	windowPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - windowSize[0] / 2;
	windowPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - windowSize[1] / 2;
	windowHwnd = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED, wndClass.lpszClassName, OVERLAY_WINDOW_NAME, WS_POPUP, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], nullptr, nullptr, wndClass.hInstance, nullptr);

	if (windowHwnd == nullptr)
	{
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		return 2;
	}

	MARGINS overlayMargins = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(windowHwnd, &overlayMargins);
	SetLayeredWindowAttributes(windowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA);
	SetWindowLongPtr(windowHwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT);

	return 0;
}

void ErectusMain::CloseWnd() {
	if (windowHwnd != nullptr)
	{
		SendMessage(windowHwnd, WM_CLOSE, NULL, NULL);
	}
	UnregisterClass(OVERLAY_WINDOW_CLASS, mHInstance);
}

bool ErectusMain::SetOverlayPosition(const bool topmost, const bool layered)
{
	RECT windowRect;
	RECT clientRect;

	if (!ErectusProcess::HwndValid(ErectusProcess::pid) || !GetWindowRect(ErectusProcess::hWnd, &windowRect) || !GetClientRect(ErectusProcess::hWnd, &clientRect))
	{
		overlayActive = false;
		return false;
	}

	int size[2];
	size[0] = clientRect.right;
	size[1] = clientRect.bottom;

	int position[2];
	position[0] = windowRect.left - (((clientRect.right + windowRect.left) - windowRect.right) / 2);
	position[1] = windowRect.top - (((clientRect.bottom + windowRect.top) - windowRect.bottom) / 2);

	if (GetWindowLongPtr(ErectusProcess::hWnd, GWL_STYLE) & WS_BORDER)
	{
		auto buffer = GetSystemMetrics(SM_CYCAPTION) / 2;
		buffer += (buffer & 1);
		position[1] += buffer;
	}

	if (GetMenu(ErectusProcess::hWnd) != nullptr)
	{
		auto buffer = GetSystemMetrics(SM_CYMENU) / 2;
		buffer += (buffer & 1);
		position[1] += buffer;
	}

	if (position[0] != windowPosition[0] || position[1] != windowPosition[1])
	{
		windowPosition[0] = position[0];
		windowPosition[1] = position[1];
		MoveWindow(windowHwnd, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], FALSE);
	}

	if (size[0] != windowSize[0] || size[1] != windowSize[1])
	{
		windowSize[0] = size[0];
		windowSize[1] = size[1];
		Renderer::deviceResetQueued = true;
	}

	if (topmost || layered)
	{
		auto style = GetWindowLongPtr(windowHwnd, GWL_EXSTYLE);

		if (topmost && !(style & WS_EX_TOPMOST))
		{
			SetWindowPos(windowHwnd, HWND_TOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
			windowTopmostCounter++;
			if (windowTopmostCounter > 3)
			{
				windowTopmostCounter = 0;
				ErectusProcess::SetProcessError(0, "Process State: Overlay not topmost");
				overlayActive = false;
				return false;
			}
		}
		else if (!topmost && (style & WS_EX_TOPMOST))
		{
			SetWindowPos(windowHwnd, HWND_NOTOPMOST, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
		}
		else
		{
			windowTopmostCounter = 0;
		}

		if (layered && !(style & WS_EX_LAYERED))
		{
			style |= WS_EX_LAYERED;
			SetWindowLongPtr(windowHwnd, GWL_EXSTYLE, style);
			if (experimentalOverlayFix)
			{
				SetLayeredWindowAttributes(windowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA | LWA_COLORKEY);
			}
			else
			{
				SetLayeredWindowAttributes(windowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA);
			}
		}
		else if (!layered && (style & WS_EX_LAYERED))
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(windowHwnd, GWL_EXSTYLE, style);
		}
	}

	ErectusProcess::processMenuActive = false;
	overlayMenuActive = false;
	overlayActive = true;
	return true;
}

void ErectusMain::KeybindInput(DWORD* keybindKey, DWORD* keybindBit)
{
	if (keybindHandlerKey != nullptr && keybindHandlerBit != nullptr)
	{
		*keybindHandlerKey = oldKeybindHandlerKey;
		*keybindHandlerBit = oldKeybindHandlerBit;
	}
	keybindHandlerKey = keybindKey;
	keybindHandlerBit = keybindBit;
	oldKeybindHandlerKey = *keybindHandlerKey;
	oldKeybindHandlerBit = *keybindHandlerBit;
	*keybindHandlerKey = 0;
	*keybindHandlerBit = 0;
}

void ErectusMain::CancelKeybindInput()
{
	if (keybindHandlerKey != nullptr && keybindHandlerBit != nullptr)
	{
		*keybindHandlerKey = oldKeybindHandlerKey;
		*keybindHandlerBit = oldKeybindHandlerBit;
		keybindHandlerKey = nullptr;
		keybindHandlerBit = nullptr;
		oldKeybindHandlerKey = 0;
		oldKeybindHandlerBit = 0;
	}
}

void ErectusMain::ClearKeybind(DWORD* keybindKey, DWORD* keybindBit)
{
	if (keybindHandlerKey == keybindKey && keybindHandlerBit == keybindBit)
	{
		keybindHandlerKey = nullptr;
		keybindHandlerBit = nullptr;
		oldKeybindHandlerKey = 0;
		oldKeybindHandlerBit = 0;
	}
	*keybindKey = 0;
	*keybindBit = 0;
}

bool ErectusMain::KeybindHandler(const WPARAM wParam, const LPARAM lParam)
{
	if (keybindHandlerKey == nullptr)
	{
		return false;
	}

	if (keybindHandlerBit == nullptr)
	{
		return false;
	}

	*keybindHandlerKey = DWORD(wParam);
	*keybindHandlerBit = DWORD(lParam);
	keybindHandlerKey = nullptr;
	keybindHandlerBit = nullptr;
	oldKeybindHandlerKey = 0;
	oldKeybindHandlerBit = 0;

	return true;
}




