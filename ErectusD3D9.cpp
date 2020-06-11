#include "ErectusInclude.h"

bool ErectusD3D9::D3D9DrawText(const char *Text, bool Shadowed, bool Centered, float *Position, float *Color, float Alpha)
{
	if (Text == nullptr)
	{
		return false;
	}

	if (Alpha <= 0.0f)
	{
		return false;
	}

	DWORD Format = DT_NOCLIP;
	if (Centered) Format |= DT_CENTER;

	LONG LongPosition[2] = { LONG(Position[0]), LONG(Position[1]) };

	if (Shadowed)
	{
		RECT Background = { LongPosition[0] + 1L, LongPosition[1] + 1L, LongPosition[0] + 1L, LongPosition[1] + 1L };
		D3DXFont->DrawText(D3DXSprite, Text, -1, &Background, Format, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, Alpha));
	}

	RECT Foreground = { LongPosition[0], LongPosition[1], LongPosition[0], LongPosition[1] };
	D3DXFont->DrawText(D3DXSprite, Text, -1, &Foreground, Format, D3DCOLOR_COLORVALUE(Color[0], Color[1], Color[2], Alpha));

	return true;
}

void ErectusD3D9::D3D9Reset(HRESULT DeviceState)
{
	if (DeviceState == D3DERR_DEVICELOST)
	{
		DeviceResetCounter++;
		if (DeviceResetCounter > 600)
		{
			DeviceResetCounter = 0;
			ErectusMain::Close();
		}
	}
	else
	{
		MoveWindow(ErectusMain::WindowHwnd, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], FALSE);

		D3D9Parameters.BackBufferWidth = ErectusMain::WindowSize[0];
		D3D9Parameters.BackBufferHeight = ErectusMain::WindowSize[1];

		if (DeviceResetState == false)
		{
			DeviceResetState = true;
			DeviceResetCounter = 0;
			D3DXSprite->OnLostDevice();
			D3DXFont->OnLostDevice();
			ImGui_ImplDX9_InValidateDeviceObjects();
		}

		if (D3D9Device->Reset(&D3D9Parameters) == D3D_OK)
		{
			DeviceResetState = false;
			DeviceResetQueued = false;
			D3DXSprite->OnResetDevice();
			D3DXFont->OnResetDevice();
			ImGui_ImplDX9_CreateDeviceObjects();
		}

		MoveWindow(ErectusMain::WindowHwnd, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], FALSE);
	}
}

void ErectusD3D9::D3D9Render()
{
	auto Preframe = std::chrono::system_clock::now();

	if (D3D9Initialized && ErectusImGui::ImGuiInitialized)
	{
		HRESULT DeviceState = D3D9Device->TestCooperativeLevel();
		switch (DeviceState)
		{
		case D3D_OK:
			D3D9Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);
			D3D9Device->BeginScene();
			if (ErectusProcess::ProcessMenuActive)
			{
				ErectusImGui::ProcessMenu();
			}
			else
			{
				if (ErectusMain::OverlayMenuActive)
				{
					ErectusImGui::OverlayMenu();
				}
				else if (ErectusMain::OverlayActive && ErectusMain::OverlayForeground)
				{
					D3DXSprite->Begin(D3DXSPRITE_ALPHABLEND);

					ErectusMemory::TargetLockingValid = false;
					ErectusMemory::TargetLockingClosestDegrees = ErectusIni::CustomTargetSettings.LockingFOV;
					ErectusMemory::TargetLockingClosestPtr = 0;

					ErectusMemory::RenderCustomEntityList();
					ErectusMemory::RenderCustomNPCList();
					ErectusMemory::RenderCustomPlayerList();

					if (!ErectusMemory::TargetLockingValid)
					{
						if (ErectusMemory::TargetLockingPtr)
						{
							if (ErectusIni::CustomTargetSettings.Retargeting)
							{
								ErectusMemory::TargetLockingCooldown = ErectusIni::CustomTargetSettings.Cooldown;
							}
							else
							{
								ErectusMemory::TargetLockingCooldown = -1;
							}

							ErectusMemory::TargetLockingPtr = 0;
						}
						else if (ErectusMemory::TargetLockingClosestDegrees < ErectusIni::CustomTargetSettings.LockingFOV)
						{
							ErectusMemory::TargetLockingCooldown = 0;
							ErectusMemory::TargetLockingPtr = ErectusMemory::TargetLockingClosestPtr;
						}
					}

					ErectusMemory::RenderData();

					D3DXSprite->End();
				}
			}
			D3D9Device->EndScene();
			D3D9Device->Present(NULL, NULL, NULL, NULL);
			break;
		case D3DERR_DEVICENOTRESET:
			DeviceResetQueued = true;
			break;
		case D3DERR_DEVICELOST:
			DeviceResetQueued = true;
			break;
		default:
			ErectusMain::Close();
			break;
		}

		if (ErectusProcess::ProcessSelected)
		{
			ErectusProcess::ProcessValidCounter++;
			if (ErectusProcess::ProcessValidCounter > 20)
			{
				ErectusProcess::ProcessValidCounter = 0;
				if (WaitForSingleObject(ErectusProcess::Handle, 0) != WAIT_TIMEOUT)
				{
					ErectusProcess::ResetProcessData(true, 1);
				}

				if (ErectusMain::OverlayActive)
				{
					if (ErectusProcess::Hwnd == GetForegroundWindow())
					{
						ErectusMain::OverlayForeground = true;
						if (!ErectusMain::SetOverlayPosition(true, true))
						{
							ErectusProcess::SetProcessMenu();
						}
					}
					else
					{
						ErectusMain::OverlayForeground = false;
						if (!ErectusMain::SetOverlayPosition(false, false))
						{
							ErectusProcess::SetProcessMenu();
						}
					}
				}
			}

			if (!ErectusThread::ThreadCreationState)
			{
				ErectusThread::ThreadCreationState = ErectusThread::CreateProcessThreads();
			}

			if (Erectus::DoubleKeyPress(VK_CONTROL, VK_RETURN, &ErectusMain::OverlayMenuPress))
			{
				if (ErectusMain::OverlayMenuActive)
				{
					ErectusMain::SetOverlayPosition(false, true);
				}
				else
				{
					ErectusMain::SetOverlayMenu();
				}
			}
		}

		if (DeviceResetQueued)
		{
			D3D9Reset(DeviceState);
		}

		if (ErectusThread::ThreadDestructionQueued)
		{
			if (!ErectusThread::ThreadDestruction())
			{
				ErectusThread::ThreadDestructionCounter++;
				if (ErectusThread::ThreadDestructionCounter > 900)
				{
					ErectusThread::ThreadDestructionCounter = 0;
					ErectusMain::Close();
				}
			}
		}
	}

	std::this_thread::sleep_until(Preframe + std::chrono::milliseconds(16));
}

bool ErectusD3D9::D3D9Initialize()
{
	ZeroMemory(&D3D9Parameters, sizeof(D3D9Parameters));
	D3D9Parameters.BackBufferWidth = ErectusMain::WindowSize[0];
	D3D9Parameters.BackBufferHeight = ErectusMain::WindowSize[1];
	D3D9Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	D3D9Parameters.BackBufferCount = 1;
	D3D9Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3D9Parameters.MultiSampleQuality = 0;
	D3D9Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3D9Parameters.hDeviceWindow = ErectusMain::WindowHwnd;
	D3D9Parameters.Windowed = TRUE;
	D3D9Parameters.EnableAutoDepthStencil = TRUE;
	D3D9Parameters.AutoDepthStencilFormat = D3DFMT_D16;
	D3D9Parameters.Flags = 0;
	D3D9Parameters.FullScreen_RefreshRateInHz = 0;
	D3D9Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3D9Interface = Direct3DCreate9(D3D_SDK_VERSION);
	if (D3D9Interface == NULL) return false;
	else D3D9InterfaceCreated = true;

	HRESULT DeviceResult = D3D9Interface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3D9Parameters, &D3D9Device);
	if (DeviceResult != D3D_OK) return false;
	else D3D9DeviceCreated = true;

	HRESULT SpriteResult = D3DXCreateSprite(D3D9Device, &D3DXSprite);
	if (SpriteResult != D3D_OK) return false;
	else D3DXSpriteCreated = true;

	HRESULT FontResult = D3DXCreateFont(D3D9Device, 0, 0, 0, 0, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "System", &D3DXFont);
	if (FontResult != D3D_OK) return false;
	else D3DXFontCreated = true;

	return true;
}

void ErectusD3D9::D3D9Cleanup()
{
	if (D3D9InterfaceCreated)
	{
		D3D9InterfaceCreated = false;
		D3D9Interface->Release();
		D3D9Interface = NULL;
	}

	if (D3D9DeviceCreated)
	{
		D3D9DeviceCreated = false;
		D3D9Device->Release();
		D3D9Device = NULL;
	}

	if (D3DXSpriteCreated)
	{
		D3DXSpriteCreated = false;
		D3DXSprite->Release();
		D3DXSprite = NULL;
	}

	if (D3DXFontCreated)
	{
		D3DXFontCreated = false;
		D3DXFont->Release();
		D3DXFont = NULL;
	}
}