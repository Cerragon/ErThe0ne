#pragma once
#include <memory>
#include <Windows.h>


#include "common.h"
#include "Window.hpp"

class App final {
public:
	enum class Mode
	{
		Standalone,
		Attached,
		Overlay
	};

	App(HINSTANCE hInstance, LPCSTR windowTitle);
	~App();
	[[nodiscard]] const HINSTANCE& GetAppInstance() const { return appInstance; }

	void Run();
	void Shutdown();

	void Attach(DWORD pid);
	void Detach();
	void ToggleOverlay();

	std::unique_ptr<Window> appWindow;
	Mode mode = Mode::Standalone;
	void OnWindowChanged() const;

private:
	void SetMode(Mode newMode);

	void RegisterHotkeys();
	void UnRegisterHotkeys();
	void OnHotkey(HotKey hotkey);

	void Init(LPCSTR windowTitle);

	void Update();
	void Render() const;
	bool SnapToWindow(HWND hwnd) const;

	const HINSTANCE appInstance;

	bool continueRunning = true;
	bool started = false;
};

inline App* gApp = nullptr; //fixme
