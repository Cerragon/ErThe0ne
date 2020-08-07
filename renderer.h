#pragma once
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Renderer final {
public:
	static bool Init(HWND hwnd);
	static void Shutdown();

	static bool BeginFrame();
	static bool EndFrame();

	static void Resize(unsigned width, unsigned height);

	inline static LPDIRECT3DDEVICE9EX d3dDevice = nullptr;

private:
	static bool CreateDevice(HWND hwnd);
	static void CleanupDevice();
	static void ResetDevice();

	static void LoadFonts();

	inline static D3DPRESENT_PARAMETERS d3d9Parameters = { };
	inline static LPDIRECT3D9EX d3D9Interface = nullptr;

	inline static bool resetRequested = false;

	virtual void Dummy() = 0;
};
