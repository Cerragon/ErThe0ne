#pragma once
#include <memory>
#include <Windows.h>

#include "common.h"
#include "utils.h"
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

	[[nodiscard]] Mode GetMode() const { return mode; }
	std::unique_ptr<Window> appWindow;
	
	void OnWindowChanged();

	[[nodiscard]] Vector2 GetWindowSize() const { return currentWindowSize; }

private:
	void SetMode(Mode newMode);

	void RegisterHotkeys() const;
	void UnRegisterHotkeys() const;
	void OnHotkey(HotKey hotkey);

	void Init(LPCSTR windowTitle);

	void Update();
	void Render() const;
	bool SnapToWindow(HWND hwnd) const;

	const HINSTANCE appInstance;

	bool continueRunning = true;
	bool started = false;

	Mode mode = Mode::Standalone;
	
	Vector2 currentWindowSize = {};
};

inline App* gApp = nullptr; //fixme
