#include "app.h"
#include "common.h"


// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppParameterMayBeConst
int WINAPI wWinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{

	App app(hInstance, OVERLAY_WINDOW_NAME);
	gApp = &app; //fixme

	app.Run();

	gApp = nullptr; //fixme
	return 0;
}
