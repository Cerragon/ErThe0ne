#include "renderer.h"

#include "app.h"
#include "dependencies/fmt/fmt/format.h"
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_dx9.h"
#include "dependencies/imgui/imgui_impl_win32.h"

bool Renderer::Init(const HWND hwnd)
{
	if (!CreateDevice(hwnd))
		return false;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(d3dDevice);

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::GetIO().IniFilename = nullptr;

	ImGui::GetStyle().FrameRounding = 0.f;
	ImGui::GetStyle().WindowRounding = 0.f;
	ImGui::GetStyle().ItemSpacing = { 4.f, 4.f };

	LoadFonts();

	return true;
}

bool Renderer::CreateDevice(const HWND hwnd)
{
	Direct3DCreate9Ex(D3D_SDK_VERSION, &d3D9Interface);
	if (d3D9Interface == nullptr)
		return false;

	d3d9Parameters = {
		.BackBufferFormat = D3DFMT_A8R8G8B8,
		.BackBufferCount = 0,
		.MultiSampleType = D3DMULTISAMPLE_NONE,
		.MultiSampleQuality = 0,
		.SwapEffect = D3DSWAPEFFECT_DISCARD,
		.hDeviceWindow = hwnd,
		.Windowed = 1,
		.EnableAutoDepthStencil = 1,
		.AutoDepthStencilFormat = D3DFMT_D16,
		.Flags = 0,
		.FullScreen_RefreshRateInHz = 0,
		.PresentationInterval = D3DPRESENT_INTERVAL_ONE,
	};

	if (d3D9Interface->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3d9Parameters, nullptr, &d3dDevice) != D3D_OK)
		return false;

	return true;
}

void Renderer::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDevice();
}

bool Renderer::BeginFrame()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool Renderer::EndFrame()
{
	ImGui::EndFrame();

	const auto d3dresult = d3dDevice->TestCooperativeLevel();
	if (d3dresult == D3DERR_DEVICELOST)
		return false;

	if (resetRequested || d3dresult == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
		return  false;
	}

	if (d3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		d3dDevice->EndScene();
	}
	const auto result = d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && d3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();

	return true;
}

void Renderer::CleanupDevice()
{
	if (d3dDevice)
	{
		d3dDevice->Release();
		d3dDevice = nullptr;
	}

	if (d3D9Interface)
	{
		d3D9Interface->Release();
		d3D9Interface = nullptr;
	}
}

void Renderer::ResetDevice()
{
	resetRequested = false;

	if (d3dDevice == nullptr)
		return;

	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto hr = d3dDevice->Reset(&d3d9Parameters);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	LoadFonts();
}

void Renderer::LoadFonts()
{
	auto io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
#pragma warning(suppress : 4996)
	io.Fonts->AddFontFromFileTTF(format(FMT_STRING("{}\\Fonts\\arialbd.ttf"), std::getenv("WINDIR")).c_str(), 13.f);
	ImGui_ImplDX9_CreateDeviceObjects();

}

void Renderer::Resize(const unsigned width, const unsigned height)
{
	if (d3d9Parameters.BackBufferWidth != width || d3d9Parameters.BackBufferHeight != 0)
	{
		d3d9Parameters.BackBufferWidth = width;
		d3d9Parameters.BackBufferHeight = height;

		//the actual Reset() is deferred until BeginScene(), because we might be in the middle of imgui rendering loop
		resetRequested = true;
	}
}
