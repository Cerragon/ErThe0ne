#include "ErectusInclude.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ErectusMain::Close()
{
	if (ErectusMain::WindowHwnd != NULL)
	{
		SendMessage(WindowHwnd, WM_CLOSE, NULL, NULL);
	}
}

void ErectusMain::SetOverlayMenu()
{
	if (WindowSize[0] != 480 || WindowSize[1] != 480)
	{
		WindowSize[0] = 480;
		WindowSize[1] = 720;

		if (WindowHwnd != NULL)
		{
			ErectusD3D9::DeviceResetQueued = true;
			SetWindowPos(WindowHwnd, HWND_NOTOPMOST, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], 0);
		}
	}

	int BufferPosition[2];
	BufferPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - (WindowSize[0] / 2);
	BufferPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - (WindowSize[1] / 2);

	if (WindowPosition[0] != BufferPosition[0] || WindowPosition[1] != BufferPosition[1])
	{
		WindowPosition[0] = BufferPosition[0];
		WindowPosition[1] = BufferPosition[1];

		if (WindowHwnd != NULL)
		{
			MoveWindow(WindowHwnd, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], FALSE);
			if (!ErectusD3D9::DeviceResetQueued)
			{
				SetWindowPos(WindowHwnd, HWND_NOTOPMOST, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], 0);
			}
		}
	}

	if (WindowHwnd != NULL)
	{
		LONG_PTR Style = GetWindowLongPtr(WindowHwnd, GWL_EXSTYLE);

		if (Style & WS_EX_LAYERED)
		{
			Style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(WindowHwnd, GWL_EXSTYLE, Style);
		}

		if (Style & WS_EX_TOPMOST)
		{
			SetWindowPos(WindowHwnd, HWND_NOTOPMOST, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], 0);
		}
	}

	ErectusProcess::ProcessMenuActive = false;
	OverlayMenuActive = true;
	OverlayActive = false;
}

bool ErectusMain::SetOverlayPosition(bool Topmost, bool Layered)
{
	if (!ErectusProcess::HwndValid(ErectusProcess::Pid))
	{
		OverlayActive = false;
		return false;
	}

	RECT WindowRect;
	if (!GetWindowRect(ErectusProcess::Hwnd, &WindowRect))
	{
		OverlayActive = false;
		return false;
	}

	RECT ClientRect;
	if (!GetClientRect(ErectusProcess::Hwnd, &ClientRect))
	{
		OverlayActive = false;
		return false;
	}

	int Size[2];
	Size[0] = ClientRect.right;
	Size[1] = ClientRect.bottom;

	int Position[2];
	Position[0] = WindowRect.left - (((ClientRect.right + WindowRect.left) - WindowRect.right) / 2);
	Position[1] = WindowRect.top - (((ClientRect.bottom + WindowRect.top) - WindowRect.bottom) / 2);

	if (GetWindowLongPtr(ErectusProcess::Hwnd, GWL_STYLE) & WS_BORDER)
	{
		int Buffer = GetSystemMetrics(SM_CYCAPTION) / 2;
		Buffer += (Buffer & 1);
		Position[1] += Buffer;
	}

	if (GetMenu(ErectusProcess::Hwnd) != NULL)
	{
		int Buffer = GetSystemMetrics(SM_CYMENU) / 2;
		Buffer += (Buffer & 1);
		Position[1] += Buffer;
	}

	if (Position[0] != WindowPosition[0] || Position[1] != WindowPosition[1])
	{
		WindowPosition[0] = Position[0];
		WindowPosition[1] = Position[1];
		MoveWindow(WindowHwnd, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], FALSE);
	}

	if (Size[0] != WindowSize[0] || Size[1] != WindowSize[1])
	{
		WindowSize[0] = Size[0];
		WindowSize[1] = Size[1];
		ErectusD3D9::DeviceResetQueued = true;
	}

	if (Topmost || Layered)
	{
		LONG_PTR Style = GetWindowLongPtr(WindowHwnd, GWL_EXSTYLE);

		if (Topmost && !(Style & WS_EX_TOPMOST))
		{
			SetWindowPos(WindowHwnd, HWND_TOPMOST, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], 0);
			WindowTopmostCounter++;
			if (WindowTopmostCounter > 3)
			{
				WindowTopmostCounter = 0;
				ErectusProcess::SetProcessError(0, "Process State: Overlay not topmost", sizeof("Process State: Overlay not topmost"));
				OverlayActive = false;
				return false;
			}
		}
		else if (!Topmost && (Style & WS_EX_TOPMOST))
		{
			SetWindowPos(WindowHwnd, HWND_NOTOPMOST, WindowPosition[0], WindowPosition[1], WindowSize[0], WindowSize[1], 0);
		}
		else
		{
			WindowTopmostCounter = 0;
		}

		if (Layered && !(Style & WS_EX_LAYERED))
		{
			Style |= WS_EX_LAYERED;
			SetWindowLongPtr(WindowHwnd, GWL_EXSTYLE, Style);
			if (ExperimentalOverlayFix)
			{
				SetLayeredWindowAttributes(WindowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA | LWA_COLORKEY);
			}
			else
			{
				SetLayeredWindowAttributes(WindowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA);
			}
		}
		else if (!Layered && (Style & WS_EX_LAYERED))
		{
			Style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(WindowHwnd, GWL_EXSTYLE, Style);
		}
	}

	ErectusProcess::ProcessMenuActive = false;
	OverlayMenuActive = false;
	OverlayActive = true;
	return true;
}

void ErectusMain::KeybindInput(DWORD *KeybindKey, DWORD *KeybindBit)
{
	if (KeybindHandlerKey != nullptr && KeybindHandlerBit != nullptr)
	{
		*KeybindHandlerKey = OldKeybindHandlerKey;
		*KeybindHandlerBit = OldKeybindHandlerBit;
	}
	KeybindHandlerKey = KeybindKey;
	KeybindHandlerBit = KeybindBit;
	OldKeybindHandlerKey = *KeybindHandlerKey;
	OldKeybindHandlerBit = *KeybindHandlerBit;
	*KeybindHandlerKey = 0;
	*KeybindHandlerBit = 0;
}

void ErectusMain::CancelKeybindInput()
{
	if (KeybindHandlerKey != nullptr && KeybindHandlerBit != nullptr)
	{
		*KeybindHandlerKey = OldKeybindHandlerKey;
		*KeybindHandlerBit = OldKeybindHandlerBit;
		KeybindHandlerKey = nullptr;
		KeybindHandlerBit = nullptr;
		OldKeybindHandlerKey = 0;
		OldKeybindHandlerBit = 0;
	}
}

void ErectusMain::ClearKeybind(DWORD *KeybindKey, DWORD *KeybindBit)
{
	if (KeybindHandlerKey == KeybindKey && KeybindHandlerBit == KeybindBit)
	{
		KeybindHandlerKey = nullptr;
		KeybindHandlerBit = nullptr;
		OldKeybindHandlerKey = 0;
		OldKeybindHandlerBit = 0;
	}
	*KeybindKey = 0;
	*KeybindBit = 0;
}

bool ErectusMain::KeybindHandler(WPARAM wParam, LPARAM lParam)
{
	if (KeybindHandlerKey == nullptr)
	{
		return false;
	}

	if (KeybindHandlerBit == nullptr)
	{
		return false;
	}

	*KeybindHandlerKey = DWORD(wParam);
	*KeybindHandlerBit = DWORD(lParam);
	KeybindHandlerKey = nullptr;
	KeybindHandlerBit = nullptr;
	OldKeybindHandlerKey = 0;
	OldKeybindHandlerBit = 0;

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return 1;
	}

	switch (uMsg)
	{
	case WM_KEYDOWN:
		ErectusMain::KeybindHandler(wParam, lParam);
		return 0;
	case WM_PAINT:
		ErectusD3D9::D3D9Render();
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	WNDCLASSEX WindowClassEx;
	WindowClassEx.cbSize = sizeof(WNDCLASSEX);
	WindowClassEx.style = CS_VREDRAW | CS_HREDRAW;
	WindowClassEx.lpfnWndProc = WindowProc;
	WindowClassEx.cbClsExtra = 0;
	WindowClassEx.cbWndExtra = 0;
	WindowClassEx.hInstance = hInstance;
	WindowClassEx.hIcon = LoadIcon(WindowClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WindowClassEx.hCursor = NULL;
	WindowClassEx.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	WindowClassEx.lpszMenuName = NULL;
	WindowClassEx.lpszClassName = "ErCLS";
	WindowClassEx.hIconSm = NULL;

	if (!RegisterClassEx(&WindowClassEx))
	{
		return 1;
	}

	ErectusMain::WindowSize[0] = 384;
	ErectusMain::WindowSize[1] = 224;
	ErectusMain::WindowPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - (ErectusMain::WindowSize[0] / 2);
	ErectusMain::WindowPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - (ErectusMain::WindowSize[1] / 2);
	ErectusMain::WindowHwnd = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED, WindowClassEx.lpszClassName, "Er", WS_POPUP, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], NULL, NULL, WindowClassEx.hInstance, NULL);
	
	if (ErectusMain::WindowHwnd == NULL)
	{
		UnregisterClass(WindowClassEx.lpszClassName, WindowClassEx.hInstance);
		return 2;
	}

	MARGINS OverlayMargins = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(ErectusMain::WindowHwnd, &OverlayMargins);
	SetLayeredWindowAttributes(ErectusMain::WindowHwnd, RGB(0x00, 0x00, 0x00), 0xFF, LWA_ALPHA);
	SetWindowLongPtr(ErectusMain::WindowHwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT);
	ShowWindow(ErectusMain::WindowHwnd, SW_SHOW);

	ErectusD3D9::D3D9Initialized = ErectusD3D9::D3D9Initialize();
	if (!ErectusD3D9::D3D9Initialized)
	{
		ErectusMain::Close();
		ErectusD3D9::D3D9Cleanup();
		UnregisterClass(WindowClassEx.lpszClassName, WindowClassEx.hInstance);
		return 3;
	}

	ErectusImGui::ImGuiInitialized = ErectusImGui::ImGuiInitialize();
	if (!ErectusImGui::ImGuiInitialized)
	{
		ErectusMain::Close();
		ErectusD3D9::D3D9Cleanup();
		ErectusImGui::ImGuiCleanup();
		UnregisterClass(WindowClassEx.lpszClassName, WindowClassEx.hInstance);
		return 4;
	}

	ErectusProcess::ResetProcessData(true, 1);
	ErectusIni::ReadIniSettings();

	MSG OverlayMsg;
	while (GetMessage(&OverlayMsg, NULL, 0, 0))
	{
		TranslateMessage(&OverlayMsg);
		DispatchMessage(&OverlayMsg);
	}

	ErectusProcess::ResetProcessData(true, 0);
	ErectusMain::CancelKeybindInput();
	ErectusIni::WriteIniSettings();

	ErectusD3D9::D3D9Cleanup();
	ErectusImGui::ImGuiCleanup();
	UnregisterClass(WindowClassEx.lpszClassName, WindowClassEx.hInstance);
	return 0;
}
