#include "app.h"
#include "gui.h"
#include "renderer.h"
#include "common.h"
#include "settings.h"
#include "threads.h"

#include <thread>

#include "ErectusProcess.h"
#include "Window.hpp"
#include "features/Looter.h"

App::App(const HINSTANCE hInstance, const LPCSTR windowTitle) : appInstance(hInstance)
{
	Init(windowTitle);
}

App::~App()
{
	Detach();
	Renderer::Shutdown();
	Settings::Write();
}

void App::Init(LPCSTR windowTitle)
{
	appWindow = std::make_unique<Window>(this, windowTitle);

	if (!Renderer::Init(appWindow->GetHwnd()))
		return;

	Settings::Read();
}

void App::Shutdown()
{
	continueRunning = false;

	//everything should be  handled by destructor...
}

void App::Run()
{
	started = true;

	MSG msg;

	while (continueRunning)
	{
		Update();
		Render();

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message)
			{
			case WM_QUIT:
				continueRunning = false;
				break;
			case WM_HOTKEY:
				OnHotkey(static_cast<HotKey>(msg.wParam));
				break;
			default:
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}

void App::OnWindowChanged()
{
	if (!started)
		return;

	const auto [width, height] = appWindow->GetSize();
	
	Renderer::Resize(width, height);

	currentWindowSize = { static_cast<float>(width), static_cast<float>(height) };
}

void App::SetMode(const Mode newMode)
{
	switch (newMode)
	{
	case Mode::Standalone:
		UnRegisterHotkeys();
		appWindow->SetStyle(Window::Styles::Standalone);
		mode = Mode::Standalone;
		break;
	case Mode::Overlay:
		RegisterHotkeys();
		appWindow->SetStyle(Window::Styles::Overlay);
		SetForegroundWindow(ErectusProcess::hWnd);
		mode = Mode::Overlay;
		break;
	case Mode::Attached:
		appWindow->SetStyle(Window::Styles::Attached);

		if (!SnapToWindow(ErectusProcess::hWnd))
		{
			Detach();
			return;
		}
		RegisterHotkeys();
		SetForegroundWindow(appWindow->GetHwnd());
		mode = Mode::Attached;
		break;
	}
}

void App::Attach(const DWORD pid)
{
	if (!ErectusProcess::AttachToProcess(pid))
	{
		Detach();
		return;
	}
	SetMode(Mode::Attached);
}

void App::Detach()
{
	while (!Threads::ThreadDestruction() && Threads::threadDestructionCounter < 900)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	ErectusProcess::ResetProcessData();

	SetMode(Mode::Standalone);
}

void App::OnHotkey(const HotKey hotkey)
{
	auto* const foregroundWnd = GetForegroundWindow();
	if (foregroundWnd != appWindow->GetHwnd() && foregroundWnd != ErectusProcess::hWnd)
		return;
	
	switch (hotkey)
	{
	case HotKey::PositionSpoofingToggle:
		if (Settings::localPlayer.positionSpoofingEnabled)
			Threads::positionSpoofingToggle = !Threads::positionSpoofingToggle;
		break;
	case HotKey::NoclipToggle:
		if (Settings::localPlayer.noclipEnabled)
			Threads::noclipToggle = !Threads::noclipToggle;
		break;
	case HotKey::OpkNpcsToggle:
		if (Settings::opk.enabled)
			Threads::opkNpcsToggle = !Threads::opkNpcsToggle;
		break;
	case HotKey::Loot:
		if (Settings::looter.mode == LooterSettings::Mode::Keybind)
			Looter::RequestLootItems();
		break;
	case HotKey::ToggleOverlay:
		ToggleOverlay();
		break;
	}
}

void App::UnRegisterHotkeys() const
{
	if (!appWindow)
		return;

	for (const auto& [hotkeyId, hotkey] : HOTKEYS)
	{
		UnregisterHotKey(appWindow->GetHwnd(), static_cast<int>(hotkeyId));
	}
}
void App::RegisterHotkeys() const
{
	for (const auto& [hotkeyId, hotkey] : HOTKEYS)
	{
		RegisterHotKey(appWindow->GetHwnd(), static_cast<int>(hotkeyId), hotkey.modifiers, hotkey.vk);
	}
}

void App::ToggleOverlay()
{
	switch (mode)
	{
	case Mode::Attached:
		SetMode(Mode::Overlay);
		break;
	case Mode::Overlay:
		SetMode(Mode::Attached);
		break;
	case Mode::Standalone:
		break;
	}
}
void App::Update()
{
	if (mode != Mode::Standalone && !IsWindow(ErectusProcess::hWnd))
	{
		Detach();
		return;
	}

	if (mode != Mode::Standalone)
		Threads::CreateProcessThreads();

	if (Threads::threadDestructionQueued)
	{
		if (!Threads::ThreadDestruction())
		{
			Threads::threadDestructionCounter++;
			if (Threads::threadDestructionCounter > 900)
			{
				Threads::threadDestructionCounter = 0;
				Shutdown();
			}
		}
	}
}

void App::Render() const
{
	Renderer::BeginFrame();
	if (mode == Mode::Attached)
		Renderer::d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(128, 0, 0, 0), 1.0f, 0);

	if (mode == Mode::Overlay)
		Renderer::d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	Gui::Render();
	Renderer::EndFrame();
}

bool App::SnapToWindow(const HWND hwnd) const
{
	RECT targetClientRect;
	if (!GetClientRect(hwnd, &targetClientRect) || IsRectEmpty(&targetClientRect))
		return false;

	appWindow->SetSize(targetClientRect.right - targetClientRect.left, targetClientRect.bottom - targetClientRect.top);

	ClientToScreen(hwnd, reinterpret_cast<POINT*>(&targetClientRect));
	appWindow->SetPosition(targetClientRect.left, targetClientRect.top);

	return true;
}
