#include "app.h"
#include "renderer.h"
#include "settings.h"
#include "gui.h"

#include "ErectusProcess.h"

namespace {

	void shutdown() {
		ErectusProcess::ResetProcessData();

		Renderer::Cleanup();
		Gui::Shutdown();

		Settings::Write();

		App::CloseWnd();
	}

	int init(const HINSTANCE hInstance) {
		App::CreateWnd(hInstance);

		ShowWindow(App::appHwnd, SW_SHOW);

		if (!Renderer::Init())
		{
			shutdown();
			return 3;
		}

		if (!Gui::Init())
		{
			shutdown();
			return 4;
		}

		Settings::Read();

		ErectusProcess::SetProcessMenu();
		return 0;
	}
}

// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppParameterMayBeConst
int WINAPI wWinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	int result;

	if (((result = init(hInstance))) && result != 0)
		return result;

	App::RegisterHotkeys();
	
	MSG overlayMsg;
	while (GetMessage(&overlayMsg, nullptr, 0, 0))
	{
		TranslateMessage(&overlayMsg);
		DispatchMessage(&overlayMsg);
	}

	shutdown();

	return 0;
}
