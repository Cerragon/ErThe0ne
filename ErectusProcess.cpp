#include "app.h"
#include "renderer.h"
#include "threads.h"

#include <thread>
#include <TlHelp32.h>

#include "ErectusProcess.h"
#include "ErectusMemory.h"


void ErectusProcess::SetProcessMenu()
{
	if (processMenuActive)
		return;
	
	if (App::windowSize[0] != 384 || App::windowSize[1] != 224)
	{
		App::windowSize[0] = 384;
		App::windowSize[1] = 224;

		if (App::appHwnd != nullptr)
		{
			Renderer::deviceResetQueued = true;
			SetWindowPos(App::appHwnd, HWND_NOTOPMOST, App::windowPosition[0], App::windowPosition[1], App::windowSize[0], App::windowSize[1], 0);
		}
	}

	int bufferPosition[2];
	bufferPosition[0] = GetSystemMetrics(SM_CXSCREEN) / 2 - App::windowSize[0] / 2;
	bufferPosition[1] = GetSystemMetrics(SM_CYSCREEN) / 2 - App::windowSize[1] / 2;

	if (App::windowPosition[0] != bufferPosition[0] || App::windowPosition[1] != bufferPosition[1])
	{
		App::windowPosition[0] = bufferPosition[0];
		App::windowPosition[1] = bufferPosition[1];

		if (App::appHwnd != nullptr)
		{
			MoveWindow(App::appHwnd, App::windowPosition[0], App::windowPosition[1], App::windowSize[0], App::windowSize[1], FALSE);
			if (!Renderer::deviceResetQueued)
			{
				SetWindowPos(App::appHwnd, HWND_NOTOPMOST, App::windowPosition[0], App::windowPosition[1], App::windowSize[0], App::windowSize[1], 0);
			}
		}
	}

	if (App::appHwnd != nullptr)
	{
		auto style = GetWindowLongPtr(App::appHwnd, GWL_EXSTYLE);

		if (style & WS_EX_LAYERED)
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(App::appHwnd, GWL_EXSTYLE, style);
		}

		if (style & WS_EX_TOPMOST)
		{
			SetWindowPos(App::appHwnd, HWND_NOTOPMOST, App::windowPosition[0], App::windowPosition[1], App::windowSize[0], App::windowSize[1], 0);
		}
	}

	processMenuActive = true;
	App::overlayMenuActive = false;
	App::overlayActive = false;
}

void ErectusProcess::SetProcessError(const int errorId, const char* error)
{
	if (errorId == -1)
		return;
	processErrorId = errorId;
	processError = error;
}

void ErectusProcess::ResetProcessData()
{
	processSelected = false;

	SetProcessMenu();
	SetProcessError(0, "Process State: No process selected");

	if (Threads::threadCreationState)
	{
		auto areThreadsActive = false;

		while (!Threads::ThreadDestruction())
		{
			Threads::threadDestructionCounter++;
			if (Threads::threadDestructionCounter > 1440)
			{
				areThreadsActive = true;
				App::CloseWnd();
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		if (!areThreadsActive)
			ErectusMemory::MessagePatcher(false);
	}

	pid = 0;
	hWnd = nullptr;
	exe = 0;

	if (handle != nullptr)
	{
		CloseHandle(handle);
		handle = nullptr;
	}
}

std::vector<DWORD> ErectusProcess::GetProcesses()
{
	std::vector<DWORD> result{ 0 };

	auto* const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return result;

	PROCESSENTRY32 lppe;
	lppe.dwSize = sizeof lppe;

	while (Process32Next(hSnapshot, &lppe))
	{
		if (!strcmp(lppe.szExeFile, "Fallout76.exe"))
			result.push_back(lppe.th32ProcessID);
	}

	CloseHandle(hSnapshot);

	return result;
}

BOOL ErectusProcess::HwndEnumFunc(const HWND hwnd, const LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);

	if (lpdwProcessId == lParam)
	{
		char buffer[sizeof"Fallout76"] = { '\0' };
		if (GetClassName(hwnd, buffer, sizeof buffer))
		{
			if (!strcmp(buffer, "Fallout76"))
			{
				hWnd = hwnd;
				return FALSE;
			}
		}
	}

	hWnd = nullptr;
	return TRUE;
}

bool ErectusProcess::HwndValid(const DWORD processId)
{
	pid = processId;
	if (!pid)
	{
		SetProcessError(2, "Process State: PID (Process Id) invalid");
		return false;
	}

	EnumWindows(WNDENUMPROC(HwndEnumFunc), pid);
	if (hWnd == nullptr)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid");
		return false;
	}

	if (IsIconic(hWnd))
	{
		SetProcessError(2, "Process State: HWND (Window) minimized");
		return false;
	}

	RECT rect;
	if (GetClientRect(hWnd, &rect) == FALSE || rect.right < 16 || rect.bottom < 16)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid/minimized");
		return false;
	}

	SetProcessError(1, "Process State: Process selected");
	return true;
}

DWORD64 ErectusProcess::GetModuleBaseAddress(const DWORD pid, const char* module)
{
	auto* const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	MODULEENTRY32 lpme;
	lpme.dwSize = sizeof lpme;

	while (Module32Next(hSnapshot, &lpme))
	{
		if (!strcmp(lpme.szModule, module))
		{
			CloseHandle(hSnapshot);
			return DWORD64(lpme.modBaseAddr);
		}
	}

	CloseHandle(hSnapshot);
	return 0;
}

bool ErectusProcess::AttachToProcess(const DWORD processId)
{
	if(pid == processId)
		return  true;

	ResetProcessData();

	if (processId == 0)
		return false;
	
	pid = processId;

	exe = GetModuleBaseAddress(pid, "Fallout76.exe");
	if (!exe)
	{
		SetProcessError(2, "Process State: Base Address invalid");
		return false;
	}

	handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (handle == nullptr)
	{
		SetProcessError(2, "Process State: HANDLE invalid");
		return false;
	}

	SetProcessError(1, "Process State: Process selected");

	processSelected = true;
	
	return true;
}
