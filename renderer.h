#pragma once
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Renderer final {
public:
	static int BeginScene();
	static void EndScene();

	static bool Init();
	static void Cleanup();
	static bool DrawTextA(const char* text, bool shadowed, bool centered, float* position, const float* color, float alpha);

	inline static LPD3DXSPRITE d3DxSprite = nullptr;
	inline static LPDIRECT3DDEVICE9 d3D9Device = nullptr;
	inline static bool deviceResetQueued = false;

private:
	static bool Reset(HRESULT deviceState);

	inline static D3DPRESENT_PARAMETERS d3D9Parameters = { };

	inline static LPDIRECT3D9 d3D9Interface = nullptr;
	inline static LPD3DXFONT d3DxFont = nullptr;

	inline static bool deviceResetState = false;
	inline static int deviceResetCounter = 0;

	virtual void __dummy() = 0;
};
