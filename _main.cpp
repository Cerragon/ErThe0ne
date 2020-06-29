#include "app.h"
#include "renderer.h"
#include "settings.h"
#include "gui.h"

#include "ErectusProcess.h"


void onShutdown() {
	ErectusProcess::ResetProcessData();
	
	Renderer::Cleanup();
	Gui::ImGuiCleanup();

	Settings::WriteIniSettings();

	App::CloseWnd();
}

int onStartup(const HINSTANCE hInstance) {
	App::CreateWnd(hInstance);

	ShowWindow(App::appHwnd, SW_SHOW);

	if (!Renderer::Init())
	{
		onShutdown();
		return 3;
	}

	if (!Gui::ImGuiInitialize())
	{
		onShutdown();
		return 4;
	}

	Settings::ReadIniSettings();

	ErectusProcess::SetProcessMenu();
	return 0;
}


// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppParameterMayBeConst
int WINAPI wWinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	int result;

	if (((result = onStartup(hInstance))) && result != 0)
		return result;

	App::RegisterHotkeys();
	
	MSG overlayMsg;
	while (GetMessage(&overlayMsg, nullptr, 0, 0))
	{
		TranslateMessage(&overlayMsg);
		DispatchMessage(&overlayMsg);
	}

	onShutdown();

	return result;
}
