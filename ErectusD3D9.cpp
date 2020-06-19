#include "ErectusInclude.h"
#include "imgui_impl_dx9.h"

bool Renderer::DrawText(const char* text, const bool shadowed, const bool centered, float* position, const float* color,
                        const float alpha)
{
	if (text == nullptr)
	{
		return false;
	}

	if (alpha <= 0.0f)
	{
		return false;
	}

	DWORD format = DT_NOCLIP;
	if (centered) format |= DT_CENTER;

	LONG longPosition[2] = {LONG(position[0]), LONG(position[1])};

	if (shadowed)
	{
		RECT background = {longPosition[0] + 1L, longPosition[1] + 1L, longPosition[0] + 1L, longPosition[1] + 1L};
		d3DxFont->DrawText(d3DxSprite, text, -1, &background, format, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, alpha));
	}

	RECT foreground = {longPosition[0], longPosition[1], longPosition[0], longPosition[1]};
	d3DxFont->DrawText(d3DxSprite, text, -1, &foreground, format,
	                   D3DCOLOR_COLORVALUE(color[0], color[1], color[2], alpha));

	return true;
}

bool Renderer::Reset(const HRESULT deviceState)
{
	if (deviceState == D3DERR_DEVICELOST)
	{
		deviceResetCounter++;
		if (deviceResetCounter > 600)
		{
			deviceResetCounter = 0;
			return false;
		}
	}
	else
	{
		MoveWindow(ErectusMain::windowHwnd, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1],
		           ErectusMain::windowSize[0], ErectusMain::windowSize[1], FALSE);

		d3D9Parameters.BackBufferWidth = ErectusMain::windowSize[0];
		d3D9Parameters.BackBufferHeight = ErectusMain::windowSize[1];

		if (deviceResetState == false)
		{
			deviceResetState = true;
			deviceResetCounter = 0;
			d3DxSprite->OnLostDevice();
			d3DxFont->OnLostDevice();
			ImGui_ImplDX9_InvalidateDeviceObjects();
		}

		if (d3D9Device->Reset(&d3D9Parameters) == D3D_OK)
		{
			deviceResetState = false;
			deviceResetQueued = false;
			d3DxSprite->OnResetDevice();
			d3DxFont->OnResetDevice();
			ImGui_ImplDX9_CreateDeviceObjects();
		}

		MoveWindow(ErectusMain::windowHwnd, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1],
		           ErectusMain::windowSize[0], ErectusMain::windowSize[1], FALSE);
	}

	return true;
}

int Renderer::BeginScene()
{
	if (!d3D9Initialized || !ErectusImGui::imGuiInitialized)
	{
		return 1;
	}


	const auto deviceState = d3D9Device->TestCooperativeLevel();

	if (deviceResetQueued)
	{
		if (Reset(deviceState))
			return 1;
		
		return 99;
	}

	switch (deviceState)
	{
	case D3D_OK:
		d3D9Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);
		d3D9Device->BeginScene();

		return 0;

	case D3DERR_DEVICENOTRESET:
	case D3DERR_DEVICELOST:
		deviceResetQueued = true;
		return 1;
	default:
		return 99;
	}
}

void Renderer::EndScene()
{
	d3D9Device->EndScene();
	d3D9Device->Present(nullptr, nullptr, nullptr, nullptr);
}

bool Renderer::Init()
{
	ZeroMemory(&d3D9Parameters, sizeof(d3D9Parameters));
	d3D9Parameters.BackBufferWidth = ErectusMain::windowSize[0];
	d3D9Parameters.BackBufferHeight = ErectusMain::windowSize[1];
	d3D9Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3D9Parameters.BackBufferCount = 1;
	d3D9Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3D9Parameters.MultiSampleQuality = 0;
	d3D9Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3D9Parameters.hDeviceWindow = ErectusMain::windowHwnd;
	d3D9Parameters.Windowed = TRUE;
	d3D9Parameters.EnableAutoDepthStencil = TRUE;
	d3D9Parameters.AutoDepthStencilFormat = D3DFMT_D16;
	d3D9Parameters.Flags = 0;
	d3D9Parameters.FullScreen_RefreshRateInHz = 0;
	d3D9Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	d3D9Interface = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3D9Interface == nullptr)
		return false;

	const auto deviceResult = d3D9Interface->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3D9Parameters, &d3D9Device);
	if (deviceResult != D3D_OK)
		return false;

	const auto spriteResult = D3DXCreateSprite(d3D9Device, &d3DxSprite);
	if (spriteResult != D3D_OK)
		return false;

	const auto fontResult = D3DXCreateFont(d3D9Device, 0, 0, 0, 0, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
	                                       DEFAULT_QUALITY, DEFAULT_PITCH, "System", &d3DxFont);
	if (fontResult != D3D_OK)
		return false;

	return true;
}

void Renderer::Cleanup()
{
	if(d3D9Interface != nullptr)
	{
		d3D9Interface->Release();
		d3D9Interface = nullptr;
	}

	if (d3D9Device != nullptr)
	{
		d3D9Device->Release();
		d3D9Device = nullptr;
	}

	if (d3DxSprite != nullptr)
	{
		d3DxSprite->Release();
		d3DxSprite = nullptr;
	}

	if (d3DxFont != nullptr)
	{
		d3DxFont->Release();
		d3DxFont = nullptr;
	}
}
