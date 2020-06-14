#include "ErectusInclude.h"

void onShutdown() {
	ErectusProcess::ResetProcessData(true, 0);
	ErectusMain::CancelKeybindInput();

	Renderer::Cleanup();
	ErectusImGui::ImGuiCleanup();

	ErectusIni::WriteIniSettings();

	ErectusMain::CloseWnd();
}

int onStartup(const HINSTANCE hInstance) {
	ErectusMain::CreateWnd(hInstance);

	ShowWindow(ErectusMain::windowHwnd, SW_SHOW);

	Renderer::d3D9Initialized = Renderer::Init();
	if (!Renderer::d3D9Initialized)
	{
		onShutdown();
		return 3;
	}

	ErectusImGui::imGuiInitialized = ErectusImGui::ImGuiInitialize();
	if (!ErectusImGui::imGuiInitialized)
	{
		onShutdown();
		return 4;
	}

	ErectusProcess::ResetProcessData(true, 1);
	ErectusIni::ReadIniSettings();

	return 0;
}


// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppParameterMayBeConst
int WINAPI wWinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	int result;

	if (((result = onStartup(hInstance))) && result != 0)
		return result;

	MSG overlayMsg;
	while (GetMessage(&overlayMsg, nullptr, 0, 0))
	{
		TranslateMessage(&overlayMsg);
		DispatchMessage(&overlayMsg);
	}

	onShutdown();

	return result;
}
